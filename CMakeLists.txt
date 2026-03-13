cmake_minimum_required(VERSION 3.16)
project(GateServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 手动指定 gRPC 和 Protobuf 路径
set(GRPC_DIR "D:/gRPC/grpc")
set(Protobuf_DIR "D:/gRPC/grpc/lib/cmake/protobuf")

# 添加查找路径
list(APPEND CMAKE_PREFIX_PATH ${GRPC_DIR})

# 查找包
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Boost 头文件路径
include_directories("D:/boost_1_89_0")

# Boost 库路径
link_directories("D:/boost_1_89_0/stage/lib")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(GateServer
        main.cpp
        CServer.cpp
        CServer.h
        HttpConnection.cpp
        HttpConnection.h
        Singleton.h
        LogicSystem.cpp
        LogicSystem.h
        const.h
        message.pb.cc
        message.grpc.pb.cc
        VerifyGrpcClient.cpp
        VerifyGrpcClient.h
        ConfigMgr.cpp
        ConfigMgr.h
        AsioIOServicePool.cpp
        AsioIOServicePool.h
)

target_link_libraries(GateServer
        gRPC::grpc++
        protobuf::libprotobuf
        Threads::Threads
        "D:/boost_1_89_0/stage/lib/libboost_filesystem-mgw14-mt-x64-1_89.a"
        ws2_32
        mswsock
)

# 复制配置文件到输出目录
add_custom_command(TARGET GateServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
        $<TARGET_FILE_DIR:GateServer>/config.ini
        COMMENT "Copying config.ini from ${CMAKE_CURRENT_SOURCE_DIR} to $<TARGET_FILE_DIR:GateServer>"
)

if (WIN32)
    target_compile_definitions(GateServer PRIVATE _WIN32_WINNT=0x600)
endif()