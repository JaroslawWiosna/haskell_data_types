#ifndef HASKELL_DATA_TYPES_HPP_
#define HASKELL_DATA_TYPES_HPP_

#include <iostream>

void hello_world() {
    std::cout << "Hello! " << '\n';
}

// MAYBE
//

template<typename T>
struct Maybe {
    bool has_value;
    T value;
};

#endif // HASKELL_DATA_TYPES_HPP_

