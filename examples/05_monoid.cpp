#include "../haskell_data_types.hpp"

#include <cassert>
#include <iostream>

using namespace haskell_data_types;

template<typename T>
bool f(T a) requires Monoid<T> {
    return {};
}

int main() {
    {
        auto empty = mempty(Any{});
        {
            auto foo = Any{false};
            auto bar = mappend(foo, empty);
    
            assert(foo == bar);
        }
        {
            auto foo = Any{true};
            auto bar = mappend(foo, empty);
    
            assert(foo == bar);
        }
        f(empty);
        f(mappend(empty, empty));
    }
}
