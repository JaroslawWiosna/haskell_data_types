#include "../haskell_data_types.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace haskell_data_types;

int main() {
    {
        auto foo = Either<int, double>{true, {}, 42.0};
        foo.r += 1;
        auto bar = fmap([](double a){return 2.0 * a;}, foo);

        assert(86.0 == bar.value());
    }
    {
        auto foo = Either<int, double>{false, 2, {}};
        foo.r += 1;
        auto bar = fmap([](double a){return 2.0 * a;}, foo);

        assert(2 == bar.value());
    }
}
