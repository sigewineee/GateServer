cmake_minimum_required(VERSION 4.1)
project(GateServer)

set(CMAKE_CXX_STANDARD 20)


include_directories("D:/boost_1_89_0")


add_executable(GateServer main.cpp
        CServer.cpp
        CServer.h
        HttpConnection.cpp
        HttpConnection.h
        Singleton.h
        LogicSystem.cpp
        LogicSystem.h)

target_link_libraries(GateServer ws2_32 mswsock)
