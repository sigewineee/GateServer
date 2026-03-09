//
// Created by MECHREVO on 2026/3/9.
//

#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
    _acceptor(_ioc,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port))
    ,_socket(ioc){

}

void CServer::Start()
{
    auto self = shared_from_this();
    _acceptor.async_accept(_socket,[self](boost::beast::error_code ec)
    {
        try
        {
            if (ec)
            {
                self->Start();
                return ;
            }
            std::make_shared<HttpConnection>(std::move(self->_socket))->Start();

            self->Start();



        }
        catch (std::exception &exp)
        {

        }
    });

}
