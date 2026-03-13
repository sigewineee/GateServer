//
// Created by MECHREVO on 2026/3/9.
//

#ifndef GATESERVER_HTTPCONNECTION_H
#define GATESERVER_HTTPCONNECTION_H

#include "const.h"


class HttpConnection :public std::enable_shared_from_this<HttpConnection>
{
public:
    friend class LogicSystem;
    HttpConnection(boost::asio::io_context &ioc);
    void Start();
    boost::asio::ip::tcp::socket& GetSocket();
private:

    void CheckDeadline();
    void WriteResponse();
    void HandleReq();
    void PreParseGetParam();
    std::string _get_url;
    std::unordered_map<std::string, std::string> _get_params;
    boost::asio::ip::tcp::socket _socket;
    boost::beast::flat_buffer _buffer{8192};
    boost::beast::http::request<boost::beast::http::dynamic_body> _request;
    boost::beast::http::response<boost::beast::http::dynamic_body> _response;
    boost::asio::steady_timer deadline_{
        _socket.get_executor(),std::chrono::seconds(60)
    };

};


#endif //GATESERVER_HTTPCONNECTION_H