//
// Created by MECHREVO on 2026/3/12.
//

#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"

RPCConPool::RPCConPool(size_t poolsize, std::string host, std::string port)
    :_poolSize(poolsize),_host(host),_port(port),_b_stop(false)
{
    for (auto i =0;i<_poolSize;i++)
    {
        std::shared_ptr<grpc::Channel> channel =grpc::CreateChannel(host+ ":"+port,grpc::InsecureChannelCredentials());
        _connections .push(VarifyService::NewStub(channel)) ;

    }


}

RPCConPool::~RPCConPool()
{
    std::lock_guard<std::mutex> lock(_mtx);
    Close();
    while (!_connections .empty())
    {
        _connections.pop();
    }

}

void RPCConPool::Close()
{
    _b_stop = true;
    _cv.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPCConPool::getConnection()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock,[this]()
    {
        if (_b_stop)
        {
            return true;
        }
        return !_connections.empty();
    });
    if (_b_stop)
    {
        return nullptr;

    }
    auto context = std::move(_connections.front());
    _connections.pop();
    return context;
}

void RPCConPool::returnConnection(std::unique_ptr<VarifyService::Stub> context)
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (_b_stop)
    {
        return ;
    }

    _connections.push(std::move(context));
    _cv.notify_one();


}

VerifyGrpcClient::VerifyGrpcClient()
{
    auto & gCfgMgr =ConfigMgr::GetInst();
    std::string host = gCfgMgr["VarifyServer"]["Host"];
    std::string port =gCfgMgr["VarifyServer"]["Port"];
    _pool.reset(new RPCConPool(5,host,port));


}
