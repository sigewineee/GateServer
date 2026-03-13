#include "ConfigMgr.h"
#include "const.h"
#include "CServer.h"

int main()
{
    ConfigMgr & gCFgMgr = ConfigMgr::GetInst();
    std::string gate_port_str = gCFgMgr["GateServer"]["Port"];
    unsigned short gate_port = atoi(gate_port_str.c_str());

    try
    {
        unsigned short port = static_cast<unsigned short>(gate_port);
        boost::asio::io_context ioc{1};
        boost::asio::signal_set signals(ioc,SIGINT,SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error,int singnal_number)
        {
            if (error)
            {
                return;
            }
            ioc.stop();

        });

        std::make_shared<CServer>(ioc,port)->Start();
        std::cout<<"GateServer listen on port "<<port<<std::endl;
        ioc.run();
    }catch (std::exception const &e)
    {
        std::cerr<<"Error:"<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }


}
