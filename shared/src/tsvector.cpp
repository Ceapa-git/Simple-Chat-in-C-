#include "tsvector.hpp"
#include "server.hpp"

template class sc::TSVector<sc::Server::client>;

namespace sc{
    template <typename T>
    void TSVector<T>::insert(const T& value){
        std::lock_guard<std::mutex> lock(this->m);
        for (auto& e: this->v){
            if (e.state == ElementState::Empty){
                e.value = value;
                e.state = ElementState::Occupied;
                return;
            }
        }
        this->v.emplace_back(element {value, ElementState::Occupied});
    }

    template <typename T>
    void TSVector<T>::remove(long unsigned int index){
        std::lock_guard<std::mutex> lock(this->m);
        if (index >= this->v.size()) throw std::out_of_range("Index out of range");
        this->v[index].state = ElementState::Empty;
    }

    template <typename T>
    T TSVector<T>::operator[](long unsigned int index){
        std::lock_guard<std::mutex> lock(this->m);
        if (index >= this->v.size()) throw std::out_of_range("Index out of range");
        if (this->v[index].state == ElementState::Empty) throw std::out_of_range("Element is empty");
        return this->v[index].value;
    }

    template <typename T>
    bool TSVector<T>::notEmpty(long unsigned int index){
        if (index >= this->v.size()) throw std::out_of_range("Index out of range");
        if (this->v[index].state == ElementState::Empty) return false;
        return true;
    }
}
