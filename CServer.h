//
// Created by MECHREVO on 2026/3/9.
//

#ifndef GATESERVER_CSERVER_H
#define GATESERVER_CSERVER_H

#include "const.h"



class CServer :public std::enable_shared_from_this<CServer>
{
public:
    CServer(boost::asio::io_context& ioc,unsigned short& port);

    void Start();

private:
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::socket _socket;

};


#endif //GATESERVER_CSERVER_H