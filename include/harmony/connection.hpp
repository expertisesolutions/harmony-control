// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef HARMONY_CONNECTION_HPP
#define HARMONY_CONNECTION_HPP

#include <boost/asio.hpp>
#include <beast/websocket.hpp>
#include <boost/signals2.hpp>

namespace harmony {

//  out: <stream:stream xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams" version="1.0" to="x.com">
//  in:  <?xml version='1.0' encoding='iso-8859-1'?><stream:stream from='x.com' id='0300797f' version='1.0' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'><stream:features><mechanisms xmlns='urn:ietf:params:xml:ns:xmpp-sasl'><mechanism>PLAIN</mechanism></mechanisms></stream:features>
//  out: <auth xmlns="urn:ietf:params:xml:ns:xmpp-sasl" mechanism="PLAIN">Z3Vlc3RAeC5jb20AZ3Vlc3QAZ3Vlc3Q=</auth>
//  in:  <success xmlns='urn:ietf:params:xml:ns:xmpp-sasl'/>
//  out: <stream:stream xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams" version="1.0" to="x.com">
//  in:  <stream:stream from='x.com' id='0300797f' version='1.0' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'><stream:features><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'/><session xmlns='urn:ietf:params:xml:nx:xmpp-session'/></stream:features>
//  out: <iq type="set" id="bind"><bind xmlns="urn:ietf:params:xml:ns:xmpp-bind"><resource>gatorade</resource></bind></iq>
//  in:  <iq id='bind' type='result'><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'><jid>1111/gatorade</jid></bind></iq>
//  out: <iq type="get" id="616184" from="guest"><oa xmlns="connect.logitech.com" mime="vnd.logitech.connect/vnd.logitech.pair">method=pair:name=harmonyjs#iOS6.0.1#iPhone</oa></iq>
//  in:  <iq/>
//  in:  <iq id="616184" to="guest" type="get"><oa xmlns='connect.logitech.com' mime='vnd.logitech.connect/vnd.logitech.pair' errorcode='200' errorstring='OK'><![CDATA[serverIdentity=2da85ac6-2999-4019-8a60-a620910f64d3:hubId=106:identity=2da85ac6-2999-4019-8a60-a620910f64d3:status=succeeded:protocolVersion={XMPP="1.0", HTTP="1.0", RF="1.0", WEBSOCKET="1.0"}:hubProfiles={Harmony="2.0"}:productId=Pimento:friendlyName=Sala baixo]]></oa></iq>
//  out: </stream:stream>
//  in:  </stream:stream>

// new connnection
//  out: <stream:stream xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams" version="1.0" to="connect.logitech.com">
//  in:  <?xml version='1.0' encoding='iso-8859-1'?><stream:stream from='connect.logitech.com' id='03007aa3' version='1.0' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'><stream:features><mechanisms xmlns='urn:ietf:params:xml:ns:xmpp-sasl'><mechanism>PLAIN</mechanism></mechanisms></stream:features>
//  out: <auth xmlns="urn:ietf:params:xml:ns:xmpp-sasl" mechanism="PLAIN">MmRhODVhYzYtMjk5OS00MDE5LThhNjAtYTYyMDkxMGY2NGQzQGNvbm5lY3QubG9naXRlY2guY29tADJkYTg1YWM2LTI5OTktNDAxOS04YTYwLWE2MjA5MTBmNjRkMwAyZGE4NWFjNi0yOTk5LTQwMTktOGE2MC1hNjIwOTEwZjY0ZDM=</auth>
//  in:  <success xmlns='urn:ietf:params:xml:ns:xmpp-sasl'/>
//  out: <stream:stream xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams" version="1.0" to="connect.logitech.com">
//  in:  <stream:stream from='connect.logitech.com' id='03007aa3' version='1.0' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'><stream:features><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'/><session xmlns='urn:ietf:params:xml:nx:xmpp-session'/></stream:features>
//  out: <iq type="set" id="bind"><bind xmlns="urn:ietf:params:xml:ns:xmpp-bind"><resource>gatorade</resource></bind></iq>
//  in:  <iq id='bind' type='result'><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'><jid>1111/gatorade</jid></bind></iq>

// teste
// <iq type="get" id="03007aa3"><oa xmlns="connect.logitech.com" mime=\"vnd.logitech.harmony/vnd.logitech.harmony.engine?config"></oa></iq>
  
// POST / HTTP/1.1
// Content-Type application/json
// charset: utf-8
// Accept: application/json
// Content-Length: 266

// {"id":"expert-home","cmd":"vnd.logitech.harmony\\/vnd.logitech.harmony.engine?holdAction","token":"NmYyMjQyMmQtM2I3Yy00MmJiLTg0MTEtMGFjOThhMmUwNWQzCg==","params":{"action":{"command":"PowerOff","type":"IRCommand","deviceId":"1"},"status":"release","timestamp":"32"}}

// POST / HTTP/1.1
// Content-Type application/json
// charset: utf-8
// Accept: application/json
// Content-Length: 266

// {"id":"expert-home","cmd":"vnd.logitech.harmony\\/vnd.logitech.harmony.engine?holdAction","token":"NmYyMjQyMmQtM2I3Yy00MmJiLTg0MTEtMGFjOThhMmUwNWQzCg==","params":{"action":{"command":"PowerOff","type":"IRCommand","deviceId":"1"},"status":"release","timestamp":"32"}}

// GET /?domain=svcs.myharmony.com&hubId=106 HTTP/1.1
// Connection: Upgrade
// Host: hubquarto:8088
// Sec-WebSocket-Key: EZk/CC77fjnnX/FrURBsMA==
// Sec-WebSocket-Version: 13
// Upgrade: websocket

// 7Tqim!N3ySg1wQ4!

// GET /?domain=svcs.myharmony.com&hubId=10601364 HTTP/1.1
// Connection: Upgrade
// Host: 192.168.33.231:8088
// Sec-WebSocket-Key: Egj/tATXq0dykrPXqjb+rA==
// Sec-WebSocket-Version: 13
// Upgrade: websocket

// email:eu@felipemagno.com.br;mode:3;accountId:9652939;ip:192.168.33.231;port:5222;uuid:6f22422d-3b7c-42bb-8411-0ac98a2e05d3;hubId:106;current_fw_version:4.13.100;productId:Pimento;setupSessionType:0;setupSessionClient:SilverlightWebClient;setupSessionIsStale:true;setupSessionSetupType:;setupStatus:0;host_name:Harmony Hub;friendlyName:Harmony Hub;discoveryServerUri:https://svcs.myharmony.com/Discovery/Discovery.svc;openApiVersion:2;minimumOpenApiClientVersionRequired:1;recommendedOpenApiClientVersion:1;protocolVersion:{XMPP="1.0", HTTP="1.0", RF="1.0", WEBSOCKET="1.0"};hubProfiles:{Harmony="2.0"};remoteId:10601364;oohEnabled:true

// POST / HTTP/1.1
// Host: 192.168.33.231:8088
// Connection: keep-alive
// Content-Length: 70
// Origin: http://sl.dhg.myharmony.com
// User-Agent: Mozilla/5.0 (Linux; Android 7.0; SM-G955N Build/NRD90M; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/61.0.3163.98 Mobile Safari/537.36 HClient/5.2.3 (Android, 7.0, samsung, SM-G955N)
// Content-Type: application/json
// Accept: */*
// Referer: http://sl.dhg.myharmony.com/mobile/2/production/_en.mobile.html?hubIP=192.168.33.231&hubName=Harmony%20Hub&locale=en-US&country=br&clientId=0d404aa1-7911-4e8f-aed7-212719ec42ec&is321&settings&is321&lipClientId=aa10d1b6-d851-444c-8c80-7f63522cc3sd&discoveryUrl=http://svcs.myharmony.com/Discovery/Discovery.svc&susChannel=Production
// Accept-Encoding: gzip, deflate
// Accept-Language: en-US
// X-Requested-With: com.logitech.harmonyhub

// {"id":49756038,"cmd":"setup.account?getProvisionInfo","timeout":90000}

// email:eu@felipemagno.com.br;mode:3;accountId:9652939;ip:192.168.33.231;port:5222;uuid:6f22422d-3b7c-42bb-8411-0ac98a2e05d3;hubId:106;current_fw_version:4.13.100;productId:Pimento;setupSessionType:0;setupSessionClient:SilverlightWebClient;setupSessionIsStale:true;setupSessionSetupType:;setupStatus:0;host_name:Harmony Hub;friendlyName:Harmony Hub;discoveryServerUri:https://svcs.myharmony.com/Discovery/Discovery.svc;openApiVersion:2;minimumOpenApiClientVersionRequired:1;recommendedOpenApiClientVersion:1;protocolVersion:{XMPP="1.0", HTTP="1.0", RF="1.0", WEBSOCKET="1.0"};hubProfiles:{Harmony="2.0"};remoteId:10601364;oohEnabled:true

// POST / HTTP/1.1
// Host: 192.168.33.231:8088
// Connection: keep-alive
// Content-Length: 54
// Origin: http://sl.dhg.myharmony.com
// User-Agent: Mozilla/5.0 (Linux; Android 7.0; SM-G955N Build/NRD90M; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/61.0.3163.98 Mobile Safari/537.36 HClient/5.2.3 (Android, 7.0, samsung, SM-G955N)
// Content-Type: application/json
// Accept: */*
// Referer: http://sl.dhg.myharmony.com/mobile/2/production/_en.mobile.html?hubIP=192.168.33.231&hubName=Harmony%20Hub&locale=en-US&country=br&clientId=0d404aa1-7911-4e8f-aed7-212719ec42ec&is321&settings&is321&lipClientId=aa10d1b6-d851-444c-8c80-7f63522cc3sd&discoveryUrl=http://svcs.myharmony.com/Discovery/Discovery.svc&susChannel=Production
// Accept-Encoding: gzip, deflate
// Accept-Language: en-US
// X-Requested-With: com.logitech.harmonyhub

// {"id":12818146,"cmd":"connect.config?get","timeout":10000}


// POST / HTTP/1.1
// Host: 192.168.33.231:8088
// Connection: keep-alive
// Content-Length: 54
// Origin: http://sl.dhg.myharmony.com
// User-Agent: Mozilla/5.0 (Linux; Android 7.0; SM-G955N Build/NRD90M; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/61.0.3163.98 Mobile Safari/537.36 HClient/5.2.3 (Android, 7.0, samsung, SM-G955N)
// Content-Type: application/json
// Accept: */*
// Referer: http://sl.dhg.myharmony.com/mobile/2/production/_en.mobile.html?hubIP=192.168.33.231&hubName=Harmony%20Hub&locale=en-US&country=br&clientId=0d404aa1-7911-4e8f-aed7-212719ec42ec&is321&settings&is321&lipClientId=aa10d1b6-d851-444c-8c80-7f63522cc3sd&discoveryUrl=http://svcs.myharmony.com/Discovery/Discovery.svc&susChannel=Production
// Accept-Encoding: gzip, deflate
// Accept-Language: en-US
// X-Requested-With: com.logitech.harmonyhub


// POST / HTTP/1.1
// Host: 192.168.33.231:8088
// Connection: keep-alive
// Content-Length: 122
// Origin: http://sl.dhg.myharmony.com
// User-Agent: Mozilla/5.0 (Linux; Android 7.0; SM-G955N Build/NRD90M; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/61.0.3163.98 Mobile Safari/537.36 HClient/5.2.3 (Android, 7.0, samsung, SM-G955N)
// Content-Type: application/json
// Accept: */*
// Referer: http://sl.dhg.myharmony.com/mobile/2/production/_en.mobile.html?hubIP=192.168.33.231&hubName=Harmony%20Hub&locale=en-US&country=br&clientId=0d404aa1-7911-4e8f-aed7-212719ec42ec&is321&settings&is321&lipClientId=aa10d1b6-d851-444c-8c80-7f63522cc3sd&discoveryUrl=http://svcs.myharmony.com/Discovery/Discovery.svc&susChannel=Production
// Accept-Encoding: gzip, deflate
// Accept-Language: en-US
// X-Requested-With: com.logitech.harmonyhub

// {"id":12818146,"cmd":"vnd.logitech.harmony/vnd.logitech.harmony.engine?config","timeout":10000}

// websocket
// {"hbus":{"id":"49316c7a03ed6bb6#dream2lteks#sm-g955n-475-7","cmd":"vnd.logitech.harmony\/vnd.logitech.harmony.engine?holdAction","params":{"action":"{\"command\":\"PowerToggle\",\"type\":\"IRCommand\",\"deviceId\":\"45235121\"}","status":"press","timestamp":10326}}}
  
struct connection
{
  connection(boost::asio::io_service& service
             , boost::asio::ip::tcp::endpoint endpoint
             , std::string_view hub_id
             , std::string_view client_id)
    : endpoint(endpoint)
    , hub_socket(service)
    , hub_websocket(hub_socket)
    , url("/?domain=svcs.myharmony.com&hubId=")
    , digest("{\"hbus\":{\"id\":\"")
    , hub_id(hub_id)
    , client_id(client_id)
  {
    digest += client_id;
    digest += "\",\"cmd\":\"connect.statedigest?get\",\"params\":{\"format\":\"json\"}}}\r\n";
    url += hub_id;
  }

  void connect(std::function<void(boost::system::error_code const& ec)> handler)
  {
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
    hub_socket.async_connect
      (endpoint, std::bind(&connection::connect_handler, this, std::placeholders::_1, handler));
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
  }

  void disconnect()
  {
    hub_socket.close();
  }

  void send(std::string_view value, boost::system::error_code& ec)
  {
    hub_websocket.write(boost::asio::const_buffers_1(value.data(), value.size()), ec);
  }
  
  void connect_handler(boost::system::error_code const& ec
                       , std::function<void(boost::system::error_code const& ec)> handler)
  {
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
    if(!ec)
    {
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
      hub_websocket.async_handshake("localhost", url
                                    , std::bind(&connection::handshake_handler, this, std::placeholders::_1, handler));
    }
    else
    {
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
      handler(ec);
    }
  }

  void handshake_handler(boost::system::error_code const& ec
                         , std::function<void(boost::system::error_code const& ec)> handler)
  {
    if(!ec)
    {
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
      hub_websocket.text(true);
      hub_websocket.write(boost::asio::buffer(digest));

      hub_websocket.async_read(read_buffer, std::bind(&connection::read_handler, this, std::placeholders::_1));
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
      handler(ec);
    }
    else
    {
    std::cout << "connection " << __FILE__ << ":" << __LINE__ << std::endl;
      handler(ec);
    }
  }

  void ping(std::function<void(boost::system::error_code const&)> handler)
  {
    hub_websocket.control_callback
      (
       [this, handler] (auto&& kind, auto&& payload)
       {
         if(kind == beast::websocket::frame_type::pong)
         {
           boost::system::error_code ec;
           handler(ec);
           hub_websocket.control_callback({});
         }
       }
      );
    hub_websocket.ping({});
  }

  void read_handler(boost::system::error_code const& ec)
  {
    if(!ec)
    {
      std::string string;
      string.reserve(read_buffer.size());
      for(auto&& buffer : read_buffer.data())
      {
        const char* data = boost::asio::buffer_cast<const char*>(buffer);
        std::copy(data, data + boost::asio::buffer_size(buffer), std::back_inserter(string));
      }
      read_buffer.consume(string.size());
      read_signal(string);
      hub_websocket.async_read(read_buffer, std::bind(&connection::read_handler, this, std::placeholders::_1));
    }
    else
    {
    }
  }

  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::socket hub_socket;
  beast::websocket::stream<boost::asio::ip::tcp::socket&> hub_websocket;
  std::string url, digest, hub_id, client_id;

  beast::multi_buffer read_buffer;
  boost::signals2::signal<void(std::string_view)> read_signal;
};
  
}

#endif

