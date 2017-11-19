// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef HARMONY_COMMUNICATION_HPP
#define HARMONY_COMMUNICATION_HPP

#include <harmony/error.hpp>
#include <harmony/connection.hpp>

#include <boost/spirit/home/x3.hpp>

#include <json.hpp>

namespace harmony {

struct communication
{
  communication(struct connection& con, std::string_view hub_id, std::string_view client_id)
    : con(&con), hub_id(hub_id), client_id(client_id)
  {
  }

  void connect(std::function<void(boost::system::error_code const&)> handler)
  {
    connect_signal_connection = con->read_signal
      .connect(std::bind(&communication::connect_handler, this, std::placeholders::_1
                         , handler
                         ));
    con->connect([handler, this] (auto& ec)
                 {
                   if(ec)
                   {
                     connect_signal_connection.disconnect();
                     handler(ec);
                   }
                 });
  }

  void disconnect()
  {
    con->disconnect();
  }

  void connect_handler(std::string_view json
                       , std::function<void(boost::system::error_code const&)> handler)
  {
    connect_signal_connection.disconnect();

    nlohmann::json j = nlohmann::json::parse(json.begin(), json.end());
    if(j["msg"] == "OK")
    {
      config_signal_connection = con->read_signal.connect
        (std::bind(&communication::config_handler, this, std::placeholders::_1, handler));

      std::string config_cmd("{\"hbus\":{\"id\":\"");
      config_cmd += client_id;
      config_cmd += "\",\"cmd\":\"vnd.logitech.harmony/vnd.logitech.harmony.engine?config\"}}";
      boost::system::error_code ec;
      con->send(config_cmd, ec);
      if(ec)
        handler(ec);
    }
    else
    {
      std::cout << "not connected" << std::endl;
    }
  }

  void idle_read_handler(std::string_view json)
  {
    std::cout << "idle read handler " << json << std::endl;
  }

  void idle_read_handler_start()
  {
    read_signal_connection = con->read_signal.connect
      (std::bind(&communication::idle_read_handler, this, std::placeholders::_1));
  }
  void idle_read_handler_pause()
  {
    read_signal_connection.disconnect();
  }

  void config_handler(std::string_view json, std::function<void(boost::system::error_code const&)> handler)
  {
    config_signal_connection.disconnect();
    config = nlohmann::json::parse(json.begin(), json.end());
    idle_read_handler_start();
    boost::system::error_code ec;
    handler(ec);
  }

  void send_ir_command(int device, std::string_view cmd
                       , std::function<void(boost::system::error_code const&)> handler)
  {
    if(sending_state)
    {
      handler(make_error_code(harmony_error_code::sending_already_started));
      return;
    }
    
    boost::system::error_code ec;
    std::string json_prefix_cmd("{\"hbus\":{\"id\":\"");
    json_prefix_cmd += client_id;
    json_prefix_cmd += "\",\"cmd\":\"vnd.logitech.harmony/vnd.logitech.harmony.engine?holdAction\",\"params\":{\"action\":\"{\\\"command\\\":\\\"";
    json_prefix_cmd += cmd;
    json_prefix_cmd += "\\\",\\\"type\\\":\\\"IRCommand\\\",\\\"deviceId\\\":\\\"";
    namespace x3 = boost::spirit::x3;
    x3::generate(std::back_inserter(json_prefix_cmd), x3::int_, device);
    json_prefix_cmd += "\\\"}\",\"status\":\"";
    std::string json_cmd = json_prefix_cmd;
    json_cmd += "press\",\"timestamp\":0}}}";

    idle_read_handler_pause();

    auto read_con = con->read_signal.connect
      ([this] (std::string_view json)
       {
         boost::system::error_code ec;
         assert(!!sending_state);
         nlohmann::json j = nlohmann::json::parse(json.begin(), json.end());
         if(!j["code"].is_null())
         {
           int err = j["code"];
           if(err != 200)
             ec = make_error_code(static_cast<harmony_error_code>(err));
         }
         else
           ec = make_error_code(harmony_error_code::unknown_error);
         sending_state->read_connection.disconnect();
         sending_state->handler(ec);
         sending_state = {};
         idle_read_handler_start();
       });
    sending_state = sending_command_state{handler, read_con};
    
    con->send(json_cmd, ec);
    if(!ec)
    {
      json_cmd = json_prefix_cmd;
      json_cmd += "release\",\"timestamp\":0}}}";
      con->send(json_cmd, ec);
      con->ping
        (
         [this] (auto& ec)
          {
            std::cout << "ping " << ec << std::endl;
            if(sending_state)
            {
              sending_state->read_connection.disconnect();
              sending_state->handler(ec);
              sending_state = {};
              idle_read_handler_start();
            }
              
          }
        );
    }
    else
    {
      handler(ec);
    }
  }

  struct connection* con;
  std::string_view hub_id, client_id;
  boost::signals2::connection connect_signal_connection, read_signal_connection
                              , config_signal_connection;
  nlohmann::json config;
  struct sending_command_state
  {
    std::function<void(boost::system::error_code const&)> handler;
    boost::signals2::connection read_connection;
  };
  std::optional<sending_command_state> sending_state;
};
  
}

#endif
