#pragma once
#include <vector>
#include <mutex>

namespace sc{
    template <typename T>
    class TSVector{
    public:
        TSVector(int size = 10): v(size, {element{}, ElementState::Empty}){}

        void insert(const T& value);
        void remove(int index);
        T operator[](int index);
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