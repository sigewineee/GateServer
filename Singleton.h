//
// Created by MECHREVO on 2026/3/9.
//

#ifndef GATESERVER_SINGLETON_H
#define GATESERVER_SINGLETON_H

#include <iostream>
#include <memory>
#include <mutex>

template <typename T>
class Singleton{
protected:
    Singleton() =  default;
    Singleton(const Singleton<T>&) = delete;
    Singleton & operator = (const Singleton <T>& st) = delete;
    static std::shared_ptr<T> _instance;

public:
    static std::shared_ptr<T> GetInst(){
        static std::once_flag s_flag;
        std::call_once(s_flag,[&](){
            _instance = std::shared_ptr<T>(new T);
        });

        return _instance;
    }

    void PrintAddress(){
        std::cout<<_instance.get()<<std::endl;
    }

    ~Singleton(){
        //delete _instance;
        std::cout<<"Singleton delete"<<std::endl;
    }



};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;







#endif //GATESERVER_SINGLETON_H