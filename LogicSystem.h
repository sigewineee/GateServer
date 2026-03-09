//
// Created by MECHREVO on 2026/3/9.
//

#ifndef GATESERVER_LOGICSYSTEM_H
#define GATESERVER_LOGICSYSTEM_H

#include "Singleton.h"
#include <functional>
#include <map>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/http/error.hpp>
#include <iostream>

class HttpConnection;
typedef  std::function<void(std::shared_ptr<HttpConnection>)>HttpHandler;
class LogicSystem :public Singleton<LogicSystem>
{
    friend class Singleton<LogicSystem>;
public:
    ~LogicSystem();
    bool HandleGet(std::string path,std::shared_ptr<HttpConnection> con);
    void RegGet(std::string url, HttpHandler handler);
private:
    LogicSystem();
    std::map<std::string,HttpHandler> _post_handlers;
    std::map<std::string,HttpHandler> _get_handlers;

};


#endif //GATESERVER_LOGICSYSTEM_H