#include "tsqueue.hpp"

namespace sc{
    template <typename T>
    void TSQueue<T>::push(const T& value){
        std::unique_lock<std::mutex> lock(this->m);
        this->q.push(value);
        lock.unlock();
        this->cv.notify_one();
    }
    template <typename T>
    void TSQueue<T>::pop(T& value){
        std::unique_lock<std::mutex> lock(this->m);
        if (this->q.empty())
            this->cv.wait(lock, [this] {return !this->q.empty();});
        value = this->q.front();
        q.pop();
    }
    template <typename T>
    bool TSQueue<T>::tryPop(T& value){
        std::unique_lock<std::mutex> lock(this->m);
        if (this->q.empty())
            return false;
        value = this->q.front();
        q.pop();
        return true;
    }

    template <typename T>
    bool TSQueue<T>::empty() const{
        std::lock_guard<std::mutex> lock(this->m);
        return this->q.empty();
    }

    template <typename T>
    TSQueue<T>& TSQueue<T>::operator<<(const T& value){
        push(value);
        return *this;
    }
    template <typename T>
    TSQueue<T>& TSQueue<T>::operator>>(T& value){
        pop(value);
        return *this;
    }
}