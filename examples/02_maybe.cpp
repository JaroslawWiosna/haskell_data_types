#include "../haskell_data_types.hpp"

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
}
