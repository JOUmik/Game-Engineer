#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <memory>

template<typename... Args>
class Delegate 
{
public:
    using CallbackType = std::function<void(Args...)>;

    void Add(CallbackType callback) 
    {
        callbacks.push_back(callback);
    }

    void RemoveAll() 
    {
        callbacks.clear();
    }

    void Broadcast(Args... args) 
    {
        for (auto& callback : callbacks) 
        {
            callback(args...);
        }
    }

private:
    std::vector<CallbackType> callbacks;
};