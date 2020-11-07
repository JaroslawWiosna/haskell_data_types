#include "../haskell_data_types.hpp"

#include <iostream>
#include <string>

int main() {
    auto foo = Either<int, double>{true, {}, 42.0};
    foo.r += 1;
}
