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
    {
        auto empty = All::mempty();
        {
            auto foo = All{false};
            auto bar = All::mappend(foo, empty);
    
            assert(foo == bar);
        }
        {
            auto foo = All{true};
            auto bar = All::mappend(foo, empty);
    
            assert(foo == bar);
        }
        f(empty);
    }
    {
        auto empty = MinInt::mempty();
        {
            auto foo = MinInt{32};
            auto bar = MinInt::mappend(foo, empty);
    
            assert(foo == bar);
        }
        {
            auto foo = MinInt{-111};
            auto bar = MinInt::mappend(foo, empty);
    
            assert(foo == bar);
        }
        f(empty);
    }
    {
        auto empty = MaxInt::mempty();
        {
            auto foo = MaxInt{32};
            auto bar = MaxInt::mappend(foo, empty);
    
            assert(foo == bar);
        }
        {
            auto foo = MaxInt{-111};
            auto bar = MaxInt::mappend(foo, empty);
    
            assert(foo == bar);
        }
        f(empty);
    }
    {
        auto empty = mempty(std::string{});
        auto foo = std::string{"Text."};
        auto bar = mappend(empty, foo);
        assert(foo == bar);
        
        assert(std::string{"Two words"} == mappend(std::string{"Two "}, std::string{"words"}));
    }
}
