#pragma once
#include <vector>
#include <mutex>

namespace sc{
    template <typename T>
    class TSVector{
    public:
        TSVector(){}

        void insert(const T& value);
        void remove(long unsigned int index);
        T operator[](long unsigned int index);
    private:
        enum class ElementState {Empty, Occupied};
        struct element{
            T value;
            ElementState state;
        };
        std::vector<element> v;
        mutable std::mutex m;
    };
}