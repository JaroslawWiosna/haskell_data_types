#include "../haskell_data_types.hpp"

#include <cassert>
#include <iostream>

int main() {
    {
        auto foo = Maybe<int>{true, 42};

        std::cout << "auto foo = Maybe<int>{true, 42};" << '\n';
        std::cout << "foo.has_value = " << foo.has_value << '\n';
        std::cout << "foo.value = " << foo.value << '\n';
        std::cout << '\n';
    }
    {
        auto foo = Maybe<int>{};

        std::cout << "auto foo = Maybe<int>{};" << '\n';
        std::cout << "foo.has_value = " << foo.has_value << '\n';
        std::cout << "foo.value = " << foo.value << '\n';
        std::cout << '\n';
    }
    {
        auto foo = Maybe<float>{};

        std::cout << "auto foo = Maybe<float>{};" << '\n';
        std::cout << "foo.has_value = " << foo.has_value << '\n';
        std::cout << "foo.value = " << foo.value << '\n';
        std::cout << '\n';
    }
    {
        auto foo = Maybe<int>{true, 42};
        auto bar = fmap([](int a){return a+1;}, foo);

        std::cout << "auto foo = Maybe<int>{true, 42};" << '\n';
        std::cout << "auto bar = fmap([](int a){return a+1;}, foo);" << '\n';
        std::cout << "bar.has_value = " << bar.has_value << '\n';
        std::cout << "bar.value = " << bar.value << '\n';
        std::cout << '\n';
        
        assert(43 == bar.value);
    }
}
