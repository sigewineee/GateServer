//
// Created by MECHREVO on 2026/3/12.
//

#include "AsioIOServicePool.h"
#include <iostream>
AsioIOServicePool::AsioIOServicePool(std::size_t size):_ioServices(size),
_workGuards(size), _nextIOService(0){
    for (std::size_t i = 0; i < size; ++i) {
        _workGuards[i] = std::make_unique<WorkGuard>(boost::asio::make_work_guard(_ioServices[i]));
    }

    //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
    for (std::size_t i = 0; i < _ioServices.size(); ++i) {
        _threads.emplace_back([this, i]() {
            _ioServices[i].run();
            });
    }
}

AsioIOServicePool::~AsioIOServicePool() {
    Stop();
    std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
    auto& service = _ioServices[_nextIOService++];
    if (_nextIOService == _ioServices.size()) {
        _nextIOService = 0;
    }
    return service;
}

void AsioIOServicePool::Stop(){
    //因为仅仅执行work.reset并不能让iocontext从run的状态中退出
    //当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
    for (auto& workGuard : _workGuards) {
        if (workGuard) {
            workGuard->reset();  // 让 io_context 可以在没有任务时退出
        }
    }

    // 停止所有 io_context
    for (auto& ioService : _ioServices) {
        if (!ioService.stopped()) {
            ioService.stop();
        }
    }

    // 等待所有线程完成
    for (auto& t : _threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}