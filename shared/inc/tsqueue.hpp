#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

namespace sc{
    template <typename T>
    class TSQueue{
    public:
        TSQueue(){}

        void push(const T& value);
        void pop(T& value);
        bool tryPop(T& value);
        bool empty() const;
        
        TSQueue& operator<<(const T& value);
        TSQueue& operator>>(T& value);
    private:
        std::queue<T> q;
        mutable std::mutex m;
        std::condition_variable cv;
    };
}