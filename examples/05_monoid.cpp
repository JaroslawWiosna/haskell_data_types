#include "../haskell_data_types.hpp"

#include <cassert>
#include <iostream>

template<typename T>
bool f(T a) requires Monoid<T> {
    return {};
}

int main() {
    {
        auto empty = Any::mempty();
        {
            auto foo = Any{false};
            auto bar = Any::mappend(foo, empty);
    
            assert(foo == bar);
        }
        {
            auto foo = Any{true};
            auto bar = Any::mappend(foo, empty);
    
            assert(foo == bar);
        }
        f(empty);
        f(mappend(empty, empty));
    }
}
