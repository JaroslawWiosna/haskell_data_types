#include "../haskell_data_types.hpp"

#include <cassert>
#include <iostream>

int main() {
    auto empty = MonoidIntPlus::mempty();
    auto foo = int{42};
    auto bar = MonoidIntPlus::mappend(foo, empty);
    assert(foo == bar);

}
