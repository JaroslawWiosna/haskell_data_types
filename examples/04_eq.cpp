#include "../haskell_data_types.hpp"

#include <iostream>

using namespace haskell_data_types;

template<typename T>
bool f(T a, T b, T c) requires Eq<T> {
    return {};
}

int main() {
        auto qqq = Either<int, int>{true, 1, 2};
        auto qqw = Either<int, int>{true, 10, 20};
        auto qqe = Either<int, int>{true, 100, 200};
        f(qqq, qqw, qqe);
}
