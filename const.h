//
// Created by MECHREVO on 2026/3/10.
//

#ifndef GATESERVER_CONST_H
#define GATESERVER_CONST_H


#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <functional>
#include <map>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/http/error.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

enum  ErrorCodes
{
    Success = 0,
    Error_Json =1001,
    RPCFailed =1002
};

class ConfigMgr;
extern ConfigMgr gCFgMgr;

#endif //GATESERVER_CONST_H