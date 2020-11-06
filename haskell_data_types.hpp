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

template<typename C, typename T>
concept Callable = requires(C c, T t) {
    { c.operator()(t) };
};

template <typename T>
auto has_inner_value(T a) = delete;

template <typename T>
auto has_inner_value(Maybe<T> a) {
    return a.has_value;
}

template <typename T>
auto inner_value(T a) = delete;

template <typename T>
auto inner_value(Maybe<T> a) {
    if (a.has_value) {
        return a.value;
    } else {
        return T{};
    }
}

template <typename T, typename C> requires Callable<C, T>
auto fmap(C fun, Maybe<T> f) {
    auto inner = inner_value(f);
    auto new_value = fun(inner);
    if (not has_inner_value(f)) {
        return Maybe<decltype(new_value)>{};
    }
    //TODO: If does not have inner value return Maybe{false};
    return Maybe<decltype(new_value)>{true, fun(inner)};
}


#endif // HASKELL_DATA_TYPES_HPP_

