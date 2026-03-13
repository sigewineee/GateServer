//
// Created by MECHREVO on 2026/3/12.
//

#ifndef GATESERVER_ASIOIOSERVICEPOOL_H
#define GATESERVER_ASIOIOSERVICEPOOL_H

#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
    friend Singleton<AsioIOServicePool>;
public:
    using IOService = boost::asio::io_context;
    // 使用 executor_work_guard 替代已废弃的 work
    using WorkGuard = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    using WorkGuardPtr = std::unique_ptr<WorkGuard>;

    ~AsioIOServicePool();
    AsioIOServicePool(const AsioIOServicePool&) = delete;
    AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

    // 使用 round-robin 的方式返回一个 io_service
    boost::asio::io_context& GetIOService();
    void Stop();

private:
    AsioIOServicePool(std::size_t size = 2);  // 默认使用2个线程，也可以使用 std::thread::hardware_concurrency()

    std::vector<IOService> _ioServices;
    std::vector<WorkGuardPtr> _workGuards;  // 改为使用 work guard
    std::vector<std::thread> _threads;
    std::size_t _nextIOService;
};

#endif //GATESERVER_ASIOIOSERVICEPOOL_H