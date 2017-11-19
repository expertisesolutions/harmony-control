
#include <harmony/communication.hpp>

#if __has_include(<variant>)
#include <variant>
#else
#include <experimental/variant>
#endif

#include <iostream>

#include <boost/asio.hpp>
#include <beast/http.hpp>
#include <beast/websocket.hpp>

namespace beast {

std::ostream& operator<<(std::ostream& os, beast::multi_buffer const& mb)
{
  auto buffers = mb.data();
  for(auto&& buffer : buffers)
  {
    const char* data = boost::asio::buffer_cast<const char*>(buffer);
    std::copy(data, data + boost::asio::buffer_size(buffer), std::ostream_iterator<char>(os));
  }
  return os;
}
  
}



int main()
{
  boost::asio::io_service io_service;

  std::string_view hub_id = "11046652"
    , client_id = "49316c7a03ed6bb6#dream2lteks#sm-g955n-475-5";
  
  
  harmony::connection connection(io_service, {boost::asio::ip::address::from_string("192.168.2.32"), 8088}
                                 , hub_id, client_id
                                 /*, [] (auto v) { std::cout << "received " << v << std::endl; }*/);

  harmony::communication com(connection, hub_id, client_id);
  com.connect([&] (auto& ec)
              {
                std::cout << "finished connection" << std::endl;
                if(!ec)
                {
                  std::cout << "send ir command" << std::endl;
                  com.send_ir_command(46949582, "VolumeUp"
                                      , [&] (auto& ec)
                                      {
                                        if(!ec)
                                        {
                                          std::cout << "no error!" << std::endl;
                                          io_service.post
                                            ([&]
                                             {
                                               boost::asio::deadline_timer timer(io_service);
                                               timer.expires_from_now(boost::posix_time::milliseconds(1000));
                                               //timer.wait();
                                               com.send_ir_command(46949582, "VolumeUp"
                                                                   , [&] (auto& ec)
                                                                   {
                                                                     std::cout << "no error?" << ec.message() << std::endl;
                                                                     com.disconnect();
                                                                   });
                                             });
                                          
                                        }
                                        else
                                        {
                                          std::cout << "error sending command: " << ec << std::endl;
                                        }
                                      });
                }
                else
                {
                  std::cout << "error ! " << ec << std::endl;
                  com.disconnect();
                }
              });


  io_service.run();
  
  // boost::asio::ip::tcp::socket hub_ws(io_service);
  // hub_ws.connect({boost::asio::ip::address::from_string("192.168.2.32"), 8088});
  
  // beast::websocket::stream<boost::asio::ip::tcp::socket&> ws(hub_ws);

  // beast::http::response<beast::http::string_body> resp;
  // try{
  //   ws.handshake(resp, "localhost", "/?domain=svcs.myharmony.com&hubId=11046652");
  // }
  // catch(std::exception& e)
  //   {
  //     std::cout <<" Error " << e.what() << std::endl;
  //     std::cout << " response " << resp << std::endl;
  //     throw e;
  //   }
  
  // ws.text(true);
 
  // //const char digest[] = "{\"id\":12818146,\"cmd\":\"connect.statedigest?get\",\"timeout\":10000}\r\n";
  // const char digest[] = "{\"hbus\":{\"id\":\"49316c7a03ed6bb6#dream2lteks#sm-g955n-475-5\",\"cmd\":\"connect.statedigest?get\",\"params\":{\"format\":\"json\"}}}\r\n";
  // ws.write(boost::asio::buffer(digest, sizeof(digest)-1));

  // std::cout << "wrote, now reading" << std::endl;
  // beast::multi_buffer buffer;
  // ws.read(buffer);
  // std::cout << "buffer " << buffer << std::endl;
  // buffer.consume(buffer.size());

  // // const char command[] =
  // //   "{\"hbus\":{\"id\":\"49316c7a03ed6bb6#dream2lteks#sm-g955n-475-7\",\"cmd\":\"vnd.logitech.harmony\/vnd.logitech.harmony.engine?holdAction\",\"params\":{\"action\":\"{\\\"command\\\":\\\"PowerToggle\\\",\\\"type\\\":\\\"IRCommand\\\",\\\"deviceId\\\":\\\"45235121\\\"}\",\"status\":\"press\",\"timestamp\":10326}}}";

  // // const char command[] =
  // //   "{\"hbus\":{\"id\":\"49316c7a03ed6bb6#dream2lteks#sm-g955n-475-7\",\"cmd\":\"vnd.logitech.harmony/vnd.logitech.harmony.engine?config\",\"params\":{\"action\":\"{\\\"command\\\":\\\"VolumeDown\\\",\\\"type\\\":\\\"IRCommand\\\",\\\"deviceId\\\":\\\"46949582\\\"}\",\"status\":\"press\",\"timestamp\":10326}}}";
  
  // const char press_cmd[] =
  //   "{\"hbus\":{\"id\":\"49316c7a03ed6bb6#dream2lteks#sm-g955n-475-7\",\"cmd\":\"vnd.logitech.harmony/vnd.logitech.harmony.engine?holdAction\",\"params\":{\"action\":\"{\\\"command\\\":\\\"VolumeDown\\\",\\\"type\\\":\\\"IRCommand\\\",\\\"deviceId\\\":\\\"46949582\\\"}\",\"status\":\"press\",\"timestamp\":10326}}}";

  // ws.write(boost::asio::buffer(press_cmd, sizeof(press_cmd)-1));

  // //sleep(1);
  
  // const char release_cmd[] =
  //   "{\"hbus\":{\"id\":\"49316c7a03ed6bb6#dream2lteks#sm-g955n-475-7\",\"cmd\":\"vnd.logitech.harmony/vnd.logitech.harmony.engine?holdAction\",\"params\":{\"action\":\"{\\\"command\\\":\\\"VolumeDown\\\",\\\"type\\\":\\\"IRCommand\\\",\\\"deviceId\\\":\\\"46949582\\\"}\",\"status\":\"release\",\"timestamp\":10326}}}";

  // ws.write(boost::asio::buffer(release_cmd, sizeof(release_cmd)-1));

  // std::cout << "reading" << std::endl;
  // ws.read(buffer);
  // std::cout << "buffer " << buffer << std::endl;
}

