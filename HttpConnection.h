//
// Created by MECHREVO on 2026/3/9.
//

#ifndef GATESERVER_HTTPCONNECTION_H
#define GATESERVER_HTTPCONNECTION_H
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>


class HttpConnection :public std::enable_shared_from_this<HttpConnection>
{
public:
    friend class LogicSystem;
    HttpConnection(boost::asio::ip::tcp::socket socket);

    void Start();

private:
    void CheckDeadline();
    void WriteResponse();
    void HandleReq();
    boost::asio::ip::tcp::socket _socket;
    boost::beast::flat_buffer _buffer{8192};
    boost::beast::http::request<boost::beast::http::dynamic_body> _request;
    boost::beast::http::response<boost::beast::http::dynamic_body> _response;
    boost::asio::steady_timer deadline_{
        _socket.get_executor(),std::chrono::seconds(60)
    };

};


#endif //GATESERVER_HTTPCONNECTION_H