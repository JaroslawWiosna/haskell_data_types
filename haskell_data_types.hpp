// Copyright 2020 Jarosław Wiosna
// 
// Permission is hereby granted, free of charge, to any person obtaining 
// a copy of this software and associated documentation files 
// (the "Software"), to deal in the Software without restriction, 
// including without limitation the rights to use, copy, modify, merge, 
// publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR 
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////
//
// https://github.com/JaroslawWiosna/haskell_data_types
//
// 
//
//////////////////////////////////////////////////////////////////////////////
//
// Changelog
//   0.2.0 maybe function
//         either function
//         value_or method for Maybe
//   0.1.1 Use only regular `mappend` and `mempty` instead of static functions
//   0.1.0 Maybe
//         Either
//         fmap for Maybe
//         fmap for Either
//         Monoid: All, Any, MinInt, MaxInt

#ifndef HASKELL_DATA_TYPES_HPP_
#define HASKELL_DATA_TYPES_HPP_

#include <iostream>
#include <utility>

void hello_world() {
    std::cout << "Hello! " << '\n';
}

// MAYBE

template<typename T>
struct Maybe {
    bool has_value{};
    T value{};

    T value_or(T default_val) {
        return (has_value ? value : default_val);
    }
};

template<typename T1, typename T2>
bool operator==(const Maybe<T1> &a, const Maybe<T2> &b) {
    if (not a.has_value && not b.has_value) {
        return true;
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

    auto value() {
        return (be_right ? r : l);
    }
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

template <typename Left, typename Right, typename C> requires Callable1<C, Right>
auto fmap(C fun, Either<Left, Right> e) {
    if (not e.be_right) {
        return e;
    }
    auto inner = e.r;
    auto new_value = fun(inner);
//    if (not has_inner_value(f)) {
//        return Maybe<decltype(new_value)>{};
//    }
    return Either<Left, decltype(new_value)>{true, e.l, new_value};
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

template <typename T> 
concept Monoid = requires (T a, T b) {
    { mempty(a) } -> std::convertible_to<T>;
    { mappend(a, b) } -> std::convertible_to<T>; 
};

// MONOID: ANY

struct Any {
    bool value;
};

static Any mempty(Any a = {}) {
    return Any{false};
}

static Any mappend(Any a, Any b) {
    return Any{a.value or b.value};
}

bool operator==(const Any &a, const Any &b) {
    return a.value == b.value;
}

// MONOID: ALL

struct All {
    bool value;
};

static All mempty(All a = {}) {
    return All{true};
}

static All mappend(All a, All b) {
    return All{a.value and b.value};
}

bool operator==(const All &a, const All &b) {
    return a.value == b.value;
}

// MONOID: MIN INT

struct MinInt {
    int value;

    static MinInt min(MinInt a, MinInt b) {
        return (a.value < b.value) ? a : b;
    }
    // TODO: replace THE_BIGGEST_INT with INT_MAX_VALUE
    static constexpr const int THE_BIGGEST_INT = 42'000;
};

static MinInt mempty(MinInt a = {}) {
    return MinInt{MinInt::THE_BIGGEST_INT};
}

static MinInt mappend(MinInt a, MinInt b) {
    return MinInt::min(a, b);
}

bool operator==(const MinInt &a, const MinInt &b) {
    return a.value == b.value;
}

// MONOID: MAX INT

struct MaxInt {
    int value;

    static MaxInt max(MaxInt a, MaxInt b) {
        return (a.value > b.value) ? a : b;
    }
    // TODO: replace THE_LOWEST_INT with INT_MAX_VALUE
    static constexpr const int THE_LOWEST_INT = -42'000;
};

static MaxInt mempty(MaxInt a = {}) {
    return MaxInt{MaxInt::THE_LOWEST_INT};
}

static MaxInt mappend(MaxInt a, MaxInt b) {
    return MaxInt::max(a, b);
}

bool operator==(const MaxInt &a, const MaxInt &b) {
    return a.value == b.value;
}

// MONOID: STRING
#if 0
struct StringMonoid {
    std::string value{};

    static StringMonoid mempty(StringMonoid a = StringMonoid{}) {
        return StringMonoid{};
    }
    static StringMonoid mappend(StringMonoid a, StringMonoid b) {
        std::string res{a.value};
        res += b.value;
        return StringMonoid{res};
    }
};

static StringMonoid mempty(StringMonoid a = StringMonoid{}) {
    return StringMonoid{};
}

static StringMonoid mappend(const StringMonoid &a, const StringMonoid &b) {
    return StringMonoid::mappend(a, b);
}

bool operator==(const StringMonoid &a, const StringMonoid &b) {
    return a.value == b.value;
}
#endif

// maybe function
// maybe :: b -> (a -> b) -> Maybe a -> b 
// maybe n _ Nothing  = n
// maybe _ f (Just x) = f x
template<typename D, typename T, typename C1> requires Callable1<C1, T>
auto maybe(D default_val, C1 c, Maybe<T> m) {
    if (m.has_value) {
        return c(m.value);
    } else {
        return default_val;
    }
}

// either function
// either                  :: (a -> c) -> (b -> c) -> Either a b -> c
// either f _ (Left x)     =  f x
// either _ g (Right y)    =  g y
template
<typename L, 
 typename R,
 typename CL,
 typename CR> requires Callable1<CL, L> && Callable1<CR, R>
auto either(CL cl, CR cr, Either<L, R> e) {
    static_assert(std::is_same<decltype(std::declval<CL>().operator()(e.l)), decltype(std::declval<CR>().operator()(e.r))>::value, "The return type of both CL function and CR function MUST be the same. This is intended behaviour of function either");
    if (e.be_right) {
        return cr(e.r);
    } else {
        return cl(e.l);
    }
}

// SEMIGROUP

template<typename S>
concept Semigroup = requires(S s1, S s2) {
    { associative_binary_operation(s1, s2) } -> std::convertible_to<S>;
};

template<typename M> requires Monoid<M>
auto associative_binary_operation(M a1, M a2) {
    return mappend(a1, a2);   
}

#endif // HASKELL_DATA_TYPES_HPP_

