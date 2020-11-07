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

template<typename C>
concept Callable0 = requires(C c) {
    { c.operator() };
};

template<typename C, typename T>
concept Callable1 = requires(C c, T t) {
    { c.operator()(t) };
};

template<typename C, typename T>
concept Callable2 = requires(C c, T t1, T t2) {
    { c.operator()(t1, t2) };
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

template <typename T, typename C> requires Callable1<C, T>
auto fmap(C fun, Maybe<T> f) {
    auto inner = inner_value(f);
    auto new_value = fun(inner);
    if (not has_inner_value(f)) {
        return Maybe<decltype(new_value)>{};
    }
    return Maybe<decltype(new_value)>{true, fun(inner)};
}

// MONOID
// Prelude Data.Monoid> :info Monoid
// class Monoid a where
//   mempty :: a
//   mappend :: a -> a -> a
//   mconcat :: [a] -> a
// 
// 0   , +  , int
// 1   , *  , int
// Inf , min, int, because min(42, Inf) == 42
// -Inf, max, int, because max(42, -Inf) == 42
// ""  , ++ , string
//
template <typename T, typename C2, typename C0> requires Callable2<C2, T> && Callable1<C0, T>
struct Monoid {
    static T mempty() {
        C0 c;
        T t;
        return c(t);
    }
    static T mappend(T a, T b) {
        C2 c{};
        T res = c(a, b);
        return {};
    }
};

//static int add(int a, int b) {
//    return a + b;
//}

#if 0
template <>
struct Monoid<int, decltype([](int a, int b){a+b;} -> int), decltype([](int x){return 0;})> {
    static int mempty() {
        return {};
    }
    static int mappend(int a, int b) {
        return {};
    }
};
#endif

using MonoidIntPlus = Monoid<int, decltype([](int a, int b){a+b;}), decltype([](int x){return 0;})>;

#if 0
template<typename C, typename T>
concept Monoid = requires(T t, T a, T b) {
    { t.mempty() };
    { t.mappend(a, b) };
};
#endif


#endif // HASKELL_DATA_TYPES_HPP_

