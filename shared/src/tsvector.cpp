#include "tsvector.hpp"

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
    void TSVector<T>::remove(int index){
        std::lock_guard<std::mutex> lock(this->m);
        if (index < 0 || index >= this->v.size()) return;
        this->v[index].state = ElementState::Empty;
    }

    template <typename T>
    T TSVector<T>::operator[](int index){
        std::lock_guard<std::mutex> lock(this->m);
        if (index < 0 || index >= this->v.size()) return nullptr;
        if (this->v[index].state == ElementState::Empty) return nullptr;
        return this->v[index].value;
    }
}
