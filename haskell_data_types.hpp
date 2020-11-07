#ifndef HASKELL_DATA_TYPES_HPP_
#define HASKELL_DATA_TYPES_HPP_

#include <iostream>

void hello_world() {
    std::cout << "Hello! " << '\n';
}

// MAYBE

template<typename T>
struct Maybe {
    bool has_value{};
    T value{};
};

template<typename T1, typename T2>
bool operator==(const Maybe<T1> &a, const Maybe<T2> &b) {
    if (not a.has_value && not b.has_value) {
        return false;
    } else if (a.has_value && b.has_value) {
        return a.value == b.value;
    } else {
        return false;
    }
}

// EITHER

template<typename Left, typename Right>
struct Either {
    bool be_right{};
    Left l{};
    Right r{};
};

template<typename Left1, typename Right1, typename Left2, typename Right2>
bool operator==(Either<Left1, Right1> &a, Either<Left2, Right2> &b) {
    if (a.be_right and b.be_right) {
        return a.r == b.r;
    }
    if ((not a.be_right) and (not b.be_right)) {
        return a.l == b.l;
    }
    return {};
}

// EQ

template<typename T>
concept Eq = requires(T a, T b) {
    { operator==(a,b) };
};

// CALLABLE

template<typename C, typename T>
concept Callable = requires(C c, T t) {
    { c.operator()(t) };
};

// HAS INNER VALUE

template <typename T>
auto has_inner_value(T a) = delete;

template <typename T>
auto has_inner_value(Maybe<T> a) {
    return a.has_value;
}

// INNER VALUE

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

// FMAP

template <typename T, typename C> requires Callable<C, T>
auto fmap(C fun, Maybe<T> f) {
    auto inner = inner_value(f);
    auto new_value = fun(inner);
    if (not has_inner_value(f)) {
        return Maybe<decltype(new_value)>{};
    }
    return Maybe<decltype(new_value)>{true, fun(inner)};
}


#endif // HASKELL_DATA_TYPES_HPP_

