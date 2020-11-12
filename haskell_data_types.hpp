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
//   0.5.0 List's head
//         List's last
//         List's tail
//         List's init
//         Wrap everything into `namespace haskell_data_types`
//   0.4.0 List
//         see CHANGELOG.md for details
//   0.3.0 flip function
//         liftA2 for Applicative
//         associative binary operation for Alternative
//         set default inner type of Maybe to int, so `Maybe{}` is now valid
//           until now one had to `auto foo = Maybe<int>{};`
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

#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

namespace haskell_data_types {

void hello_world() {
    std::cout << "Hello! " << '\n';
}

// MAYBE

template<typename T = int>
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

template<typename T1, typename T2 = T1>
concept Eq = requires(T1 a, T2 b) {
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

template<typename C, typename T1, typename T2>
concept Callable2 = requires(C c, T1 t1, T2 t2) {
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

// FLIP
// flip :: (a -> b -> c) -> b -> a -> c
template
<typename A, typename B, typename C2> requires Callable2<C2, A, B>
auto flip(C2 c2, A a, B b) {
    return c2(b, a);
}

// FUNCTOR
// class Functor f where
//    fmap :: (a -> b) -> f a -> f b
template<typename F, typename T, typename C1>
concept Functor = Callable1<C1, T> && requires(F f, C1 fun) {
    { fmap(fun, f) };
};

// APPLICATIVE
// class Functor f => Applicative f where
//    (<*>) a.k.a. liftA2 :: f (a -> b) -> f a -> f b
template<typename A, typename T, typename C1, typename T1, typename T2, typename C2>
concept Applicative = Functor<A, T, C1> && Callable2<C2, T1, T2> && requires(A a1, A a2, C2 c2) {
    { liftA2(c2, a1, a2) };
};

template<typename T1, typename T2, typename C2> requires Callable2<C2, T1, T2>
auto liftA2(C2 fun, Maybe<T1> a, Maybe<T2> b) {
    auto result = fun(a.value, b.value);
    return Maybe<decltype(result)>{(a.has_value && b.has_value), result};
}

// ALTERNATIVE
// class Applicative f => Alternative f where
//   empty :: f a
//   (<|>) :: f a -> f a -> f a -- a.k.a associative binary operation
//   some :: f a -> f [a]
//   many :: f a -> f [a]
template<typename A, typename T, typename C1, typename T1, typename T2, typename C2>
concept Alternative = Applicative<A, T, C1, T1, T2, C2> && requires(A a1, A a2) {
    { associative_binary_operation_for_alternative(a1, a2) } -> std::convertible_to<A>;
};

template<typename T>
auto associative_binary_operation_for_alternative(Maybe<T> a1, Maybe<T> a2) {
    return (a1.has_value ? a1 : a2);
}

// LIST
// It is going to be std::vector-like container (for now at least).
// We are going to support as many List interfaces from Haskell as possible
// (as long as C++ allows)

template<typename Item = int>
struct List {
    size_t capacity{0};
    size_t size{0};
    Item *data{nullptr};

    List<Item> deep_copy() {
        List<Item> copy{capacity, size};
        copy.data = (Item*)malloc(capacity * sizeof(Item));
        memcpy(copy.data, data, capacity * sizeof(Item));
        return copy;
    }

    void double_capacity() {
        capacity = (data != nullptr) ? 2 * capacity : 8;
        data = (Item*)realloc((void*)data, capacity * sizeof(Item));
    }

    void push(Item item) {
        if (size + 1 > capacity) {
            double_capacity();
        }
        memcpy(data + size++, &item, sizeof(Item));
    }

    auto operator[](int i) {
        assert(i < size);
        return data[i];
    }
};

template<typename Item, typename C1> requires Callable1<C1, Item>
auto fmap(C1 fun, List<Item> lst) {
    using Newtype = decltype(std::declval<C1>().operator()(Item{}));
    // TODO: Wouldn't it be better to zero-init. result?
    List<Newtype> result{lst.capacity, lst.size};
    result.data = (Newtype*)malloc(lst.capacity * sizeof(Newtype));
    for (int i{}; i < result.size; ++i) {
        result.data[i] = fun(lst.data[i]);
    }
    return result;
}

template<typename Item, typename C1> requires Callable1<C1, Item>
auto map(C1 fun, List<Item> lst) {
    return fmap(fun, lst);
}

template<typename Item, typename C1> requires Callable1<C1, Item>
auto filter(C1 fun, List<Item> lst) {
    List<Item> result{};
    for (int i{}; i < lst.size; ++i) {
        if (fun(lst.data[i])) {
            result.push(lst.data[i]);
        }
    }
    return result;
}

template<typename Item, typename C2> requires Callable2<C2, Item, Item>
Item foldl(C2 fun, Item init, List<Item> lst) {
    for (int i{}; i < lst.size; ++i) {
        init = fun(init, lst.data[i]);
    }
    return init;
}

template<typename T1, typename T2>
bool operator==(const List<T1> &a, const List<T2> &b) = delete;

template<typename T1, typename T2> requires Eq<T1, T2> || std::is_same<T1, T2>::value
bool operator==(const List<T1> &a, const List<T2> &b) {
    if (a.size != b.size) {
        return false;
    }
    for (size_t i; i < a.size; ++i) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }
    return true;
}

template<typename T = int>
static List<T> mempty(List<T> l = {}) {
    return List<T>{};
}

template<typename T>
static List<T> mappend(List<T> a, List<T> b) {
    for (size_t i{}; i < b.size; ++i) {
        a.push(b.data[i]);
    }
    return a;
}

template<typename T1, typename T2, typename C2> requires Callable2<C2, T1, T2>
auto liftA2(C2 fun, List<T1> a, List<T2> b) {
    using Newtype = decltype(std::declval<C2>().operator()(T1{}, T2{}));
    List<Newtype> result{};
    for (int i{}; i < a.size; ++i) {
        for (int j{}; j < b.size; ++j) {
            result.push(fun(a.data[i], b.data[j]));
        }
    }
    return result;
}

template<typename T>
auto associative_binary_operation_for_alternative(List<T> a1, List<T> a2) {
    return mappend(a1, a2);
}

template <typename T>
T head(List<T> lst) {
    assert(lst.size >= 1);
    return lst.data[0];
}

template <typename T>
T last(List<T> lst) {
    assert(lst.size >= 1);
    return lst.data[lst.size - 1];
}

template<typename T>
List<T> tail(List<T> lst) {
   assert(lst.size >= 1);
   List<T> copy{lst.capacity, lst.size - 1};
   copy.data = (T*)malloc(lst.capacity * sizeof(T));
   memcpy(copy.data, lst.data + 1, lst.capacity * sizeof(T));
   return copy; 
}

template<typename T>
List<T> init(List<T> lst) {
   assert(lst.size >= 1);
   auto result = lst.deep_copy();
   result.size--;
   return result;
}

// Haskell's List interfaces for std::vector

template<typename Item, typename C1> requires Callable1<C1, Item>
auto fmap(C1 fun, std::vector<Item> lst) {
    using Newtype = decltype(std::declval<C1>().operator()(Item{}));
    std::vector<Newtype> result{};
    for (const auto &lst_item : lst) {
        result.push_back(fun(lst_item));
    }
    return result;
}

template<typename Item, typename C1> requires Callable1<C1, Item>
auto map(C1 fun, std::vector<Item> lst) {
    return fmap(fun, lst);
}

template<typename Item, typename C1> requires Callable1<C1, Item>
auto filter(C1 fun, std::vector<Item> lst) {
    std::vector<Item> result{};
    for (const auto &lst_item : lst) {
        if (fun(lst_item)) {
            result.push_back(lst_item);
        }
    }
    return result;
}

template<typename Item, typename C2> requires Callable2<C2, Item, Item>
Item foldl(C2 fun, Item init, std::vector<Item> lst) {
    for (const auto &lst_item : lst) {
        init = fun(init, lst_item);
    }
    return init;
}

template<typename T = int>
static std::vector<T> mempty(std::vector<T> l = {}) {
    return std::vector<T>{};
}

template<typename T>
static std::vector<T> mappend(std::vector<T> a, std::vector<T> b) {
    for (const auto &b_item : b) {
        a.push_back(b_item);
    }
    return a;
}
} // namespace haskell_data_types
#endif // HASKELL_DATA_TYPES_HPP_

