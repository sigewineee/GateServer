//
// Created by MECHREVO on 2026/3/12.
//

#ifndef GATESERVER_VERIFYGRPCCLIENT_H
#define GATESERVER_VERIFYGRPCCLIENT_H

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include  "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPCConPool
{
public:
    RPCConPool(size_t poolsize ,std::string host ,std::string port);
    ~RPCConPool();
    void Close();
    std::unique_ptr<VarifyService::Stub> getConnection();
    void returnConnection(std::unique_ptr<VarifyService::Stub> context);

private:
    std::atomic<bool> _b_stop;
    size_t _poolSize;
    std::string _host;
    std::string _port;
    std::queue<std::unique_ptr<VarifyService::Stub>> _connections;
    std::mutex _mtx;
    std::condition_variable _cv;
};




class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:
    GetVarifyRsp GetVarifyCode(std::string email)
    {
        ClientContext context;
        GetVarifyRsp reply;
        GetVarifyReq request;
        request.set_email(email);
        auto stub = _pool->getConnection();
        Status status = stub ->GetVarifyCode(&context,request,&reply) ;
        if (status.ok())
        {
            _pool->returnConnection(std::move(stub));
            return reply;
        }
        else
        {
            reply.set_error(ErrorCodes::RPCFailed);
            _pool->returnConnection(std::move(stub));
            return reply;
        }

    }
    std::unique_ptr<RPCConPool> _pool;

private:
    VerifyGrpcClient();


};


#endif //GATESERVER_VERIFYGRPCCLIENT_H