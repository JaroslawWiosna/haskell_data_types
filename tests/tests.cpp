#include "../haskell_data_types.hpp"

#include <cassert>
#include <iostream>
#include <string>

size_t passed{};
size_t failed{};

void print_summary() {
    std::cout << "SUMMARY: "
        << passed
        << " passed, "
        << failed
        << " failed.\n";
}

void test(bool b) {
    (b ? ++passed : ++failed);
}

template <typename Expected, typename Actual> 
void expect_eq(Expected e, Actual a, const char *desc, const char *file, int line) {
    if (e == a) {
        ++passed;
    } else {
        ++failed;
        std::cout << file << ':' << line << ' ' << desc << "\n";
    }
}

template <typename Expected, typename Actual> 
void expect_neq(Expected e, Actual a, const char *desc, const char *file, int line) {
    if (e != a) {
        ++passed;
    } else {
        ++failed;
        std::cout << file << ':' << line << ' ' << desc << "\n";
    }
}

#define EXPECT_EQ(val1, val2, val3) \
    expect_eq((val1), (val2), (val3), __FILE__, __LINE__);

#define EXPECT_NEQ(val1, val2, val3) \
    expect_neq((val1), (val2), (val3), __FILE__, __LINE__);

namespace helper {
template<typename T>
bool this_fun_requires_monoid(T a) requires Monoid<T> {
    return {};
}
}

int main() {
    // MAYBE
    {
        auto foo = Maybe<int>{};
        auto bar = Maybe<int>{};

        EXPECT_EQ(foo, bar, "Two different instances of zero initialized maybe should be equal");
    }
    {
        auto foo = Maybe<int>{};
        auto bar = Maybe<float>{};

        EXPECT_EQ(foo, bar, "Two different instances of zero initialized maybe should be equal");
    }
    {
        auto foo = Maybe<int>{true, 42};
        auto bar = Maybe<float>{true, 42};

        EXPECT_EQ(foo, bar, "Two different instances of maybe should be equal if the inner values are equal");
    }
    {
        auto foo = Maybe<int>{};
        auto bar = Maybe<float>{true, 42};

        EXPECT_NEQ(foo, bar, "Maybe without inner value should not be equal with maybe with inner value");
    }
    // EITHER
    {
        auto foo = Either<int, double>{};
        auto bar = Either<int, double>{};

        EXPECT_EQ(foo, bar, "Zero initialized Eithers should be equal");
    }
    {
        auto foo = Either<int, double>{true, {}, {}};
        auto bar = Either<int, double>{true, {}, {}};

        EXPECT_EQ(foo, bar, "Eithers initialized as Right should be equal");
    }
    {
        auto foo = Either<int, double>{};
        auto bar = Either<int, double>{true, {}, {}};

        EXPECT_NEQ(foo, bar, "Comparing Left and Right should result in false");
    }
    // FMAP FOR MAYBE
    {
        auto foo = Maybe<int>{true, 42};
        auto bar = fmap([](int a){return 1+a;}, foo);

        EXPECT_EQ(43, bar.value, "fmap on Maybe<int>");
    }
    {
        auto foo = Maybe<int>{};
        auto bar = fmap([](int a){return 1+a;}, foo);

        EXPECT_EQ(foo, bar, "fmap on Nothing a.k.a. Maybe<T>{} should return input");
    }
    // FMAP FOR EITHER
    {
        auto foo = Either<int, double>{true, {}, 42.0};
        foo.r += 1;
        auto bar = fmap([](double a){return 2.0 * a;}, foo);

        EXPECT_EQ(86.0, bar.value(), "fmap on Right a.k.a. Either<L,R>{true}");
    }
    {
        auto foo = Either<int, double>{false, 2, {}};
        foo.r += 1;
        auto bar = fmap([](double a){return 2.0 * a;}, foo);

        EXPECT_EQ(2, bar.value(), "fmap on Left a.k.a. Either<L,R>{}");
        EXPECT_EQ(foo, bar, "fmap on Left a.k.a. Either<L,R>{} should return input");
    }
    // MONOIDS: ANY
    {
        auto empty = mempty(Any{});
        {
            auto foo = Any{false};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        {
            auto foo = Any{true};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        helper::this_fun_requires_monoid(empty);
        helper::this_fun_requires_monoid(mappend(empty, empty));
    }
    // MONOIDS: ALL
    {
        auto empty = mempty(All{});
        {
            auto foo = All{false};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        {
            auto foo = All{true};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        helper::this_fun_requires_monoid(empty);
        helper::this_fun_requires_monoid(mappend(empty, empty));
    }
    // MONOIDS: MIN INT
    {
        auto empty = mempty(MinInt{});
        {
            auto foo = MinInt{32};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        {
            auto foo = MinInt{-111};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        helper::this_fun_requires_monoid(empty);
        helper::this_fun_requires_monoid(mappend(empty, empty));
    }
    // MONOID: MAX INT
    {
        auto empty = mempty(MaxInt{});
        {
            auto foo = MaxInt{32};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        {
            auto foo = MaxInt{-111};
            auto bar = mappend(foo, empty);
    
            EXPECT_EQ(foo, bar, "mempty should be neutral");
        }
        helper::this_fun_requires_monoid(empty);
        helper::this_fun_requires_monoid(mappend(empty, empty));
    }
    // MONOID: STD::STRING
#if 0
    {
        auto empty = mempty(StringMonoid{});
        auto foo = StringMonoid{"Text."};
        auto bar = mappend(empty, foo);
        EXPECT_EQ(foo, bar, "mempty should be neutral");
        
        EXPECT_EQ(StringMonoid{"Two words"}, mappend(StringMonoid{"Two "}, StringMonoid{"words"}), "Concatenation of StringMonoid by using mappend");
        //helper::this_fun_requires_monoid(empty);
        //helper::this_fun_requires_monoid(mappend(empty, empty));
    }
#endif
    // maybe function
    {
        auto foo = Maybe<int>{true, 42};
        auto bar = maybe(1, [](int a){return a+1;}, foo);
        EXPECT_EQ(43, bar, "When foo is Just<T>, maybe fun should apply function");
    }
    {
        auto foo = Maybe<int>{};
        auto bar = maybe(1, [](int a){return a+1;}, foo);
        EXPECT_EQ(1, bar, "When foo is Nothing, maybe fun should return default value");
    }
    // either function
    {
        auto foo = Either<int, int>{false, 20, {}};
        auto bar = either([](int i){return i/2;}, [](int i){return i*2;}, foo);
        EXPECT_EQ(bar, 10, "function passed as a 1st arg to either fun should be applied to Left type");
    }
    {
        auto foo = Either<int, int>{true, {}, 100};
        auto bar = either([](int i){return i/2;}, [](int i){return i*2;}, foo);
        EXPECT_EQ(bar, 200, "function passed as a 2nd arg to either fun should be applied to Right type");
    }
    {
        auto foo = Either<int, unsigned int>{false, 20, {}};
        auto bar = either([](int i){return static_cast<float>(i/2);}, [](unsigned int i){return static_cast<float>(i);}, foo);
        EXPECT_EQ(bar, 10.f, "function passed as a 1st arg to either fun should be applied to Left type");
    }
    {
        auto foo = Either<int, unsigned int>{true, {}, 100};
        auto bar = either([](int i){return static_cast<float>(i/2);}, [](unsigned int i){return static_cast<float>(i);}, foo);
        EXPECT_EQ(bar, 100.f, "function passed as a 2nd arg to either fun should be applied to Right type");
    }
    {
        int cnt{};
        auto foo = Either<int, unsigned int>{true, {}, 100};
        auto bar = either([&](int i){++cnt; return static_cast<float>(i/2);}, [&](unsigned int i){++cnt; return static_cast<float>(i);}, foo);
        EXPECT_EQ(bar, 100.f, "function passed as a 2nd arg to either fun should be applied to Right type");
        EXPECT_EQ(1, cnt, "Only one lambda should be called. Only once.");
    }
    {
        int cnt{};
        auto foo = Either<int, unsigned int>{false, 100, {}};
        auto bar = either([&](int i){++cnt; return static_cast<float>(i/2);}, [&](unsigned int i){++cnt; return static_cast<float>(i);}, foo);
        EXPECT_EQ(bar, 50.f, "function passed as a 2nd arg to either fun should be applied to Right type");
        EXPECT_EQ(1, cnt, "Only one lambda should be called. Only once.");
    }
    // SEMIGROUP
    {
        All a{true};
        All b{true};
        All c{false};
        
        auto abo = [](auto a, auto b){return associative_binary_operation(a, b);};

        auto foo = abo(abo(a, b), c);
        auto bar = abo(a, abo(b, c));

        EXPECT_EQ(foo, bar, "The associative property of All");
    }
    {
        Any a{true};
        Any b{true};
        Any c{false};
        
        auto abo = [](auto a, auto b){return associative_binary_operation(a, b);};

        auto foo = abo(abo(a, b), c);
        auto bar = abo(a, abo(b, c));

        EXPECT_EQ(foo, bar, "The associative property of Any");
    }
    // FLIP
    {
        std::string foo{"hello"};
        std::string bar{"world"};
        
        EXPECT_EQ(std::string{"worldhello"}, flip([](std::string a, std::string b){return a+=b;}, foo, bar), "flip flips args");
    }
    // APPLICATIVE: liftA2
    {
        auto foo = Maybe<int>{true, 42};
        auto bar = Maybe<int>{true, 24};
        auto bax = Maybe<int>{};
        auto baz = Maybe<int>{};

        auto add = [](int a, int b){return a+b;};

        EXPECT_EQ((Maybe<int>{true, 66}), liftA2(add, foo, bar), "liftA2");
        EXPECT_EQ((Maybe{})             , liftA2(add, foo, baz), "liftA2");
        EXPECT_EQ((Maybe{})             , liftA2(add, bar, baz), "liftA2");
        EXPECT_EQ((Maybe{})             , liftA2(add, bax, baz), "liftA2");
    }
    // MAYBE WITH LAMBDA INSIDE AS A PARAMETER
    {
        auto foo = [](int a, int b){return a + b + 1;};
        auto bar = Maybe{true, foo};
    }
    // ALTERNATIVE
    {
        auto foo = Maybe{true, 42};
        auto bar = Maybe{true, 24};
        auto bax = Maybe{};
        auto baz = Maybe{};

        auto abofa = [](auto a, auto b){return associative_binary_operation_for_alternative(a, b);};
        EXPECT_EQ((Maybe{true, 42}), abofa(foo, bar), "Alternative");
        EXPECT_EQ((Maybe{true, 42}), abofa(foo, bax), "Alternative");
        EXPECT_EQ((Maybe{})        , abofa(bax, baz), "Alternative");
        EXPECT_EQ((Maybe{true, 24}), abofa(bax, bar), "Alternative");
        EXPECT_NEQ((Maybe{})       , abofa(bax, bar), "Alternative");
    }
    // LIST
    {
        List<int> lst{};
        for (int i{}; i < 100; ++i) {
            lst.push(i);
        }
        EXPECT_EQ(100, lst.size, "List size should be 100, because 100 Items were added");
    }
    {
        constexpr int SIZE = 5;
        List<int> lst{};
        int sum{};
        for (int i{}; i < SIZE; ++i) {
            lst.push(i);
            sum += i;
        }
        
        auto foo = [](int a){return a * 2;};
        auto mapped_lst = map(foo, lst);
        int sum_of_mapped{};
        for (int i{}; i < SIZE; ++i) {
            sum_of_mapped += mapped_lst.data[i];
        }
        
        EXPECT_EQ(sum_of_mapped, sum * 2, "List's map");
    }
    {
        List<std::string> lst{};
        lst.push("foo");
        lst.push("foofoo");

        auto bar = map([](std::string s){return s.size();}, lst);

        EXPECT_EQ(3, bar[0], "map with function that return a different type");
        EXPECT_EQ(6, bar[1], "map with function that return a different type");
        // EXPECT_NEQ(lst, bar, "List of different types should be considered equal only if both lists have the same size and operator== on every single element returns true");

        auto baz = List<float>{};
        baz.push(3.0f);
        baz.push(6);
        // EXPECT_EQ(bar, baz, "List of different types should be considered equal only if both lists have the same size and operator== on every single element returns true");

        auto bax = List<long unsigned int>{};
        bax.push(3);
        bax.push(6);
        EXPECT_EQ(bar, bax, "List of the same type should be considered equal only if both lists have the same size and operator== on every single element returns true");

    }
    {
        auto foo = List<float>{};
        foo.push(1.1f);
        foo.push(2.2f);

        auto bar = mempty(List<float>{});
        EXPECT_EQ(foo, (mappend(foo, bar)), "..");
        EXPECT_EQ(foo, (mappend(bar, foo)), "..");
        EXPECT_EQ(bar, (mappend(bar, bar)), "..");
    }
    {
        auto foo = List<float>{};
        foo.push(1.1f);
        foo.push(2.2f);

        auto bar = List<float>{};
        bar.push(100.0f);
        bar.push(200.0f);
        
        auto baz = List<unsigned int>{};
        baz.push(101);
        baz.push(201);
        baz.push(102);
        baz.push(202);

        auto add = [](float a, float b){return static_cast<unsigned int>(a+b);};

        EXPECT_EQ(baz, (liftA2(add, foo, bar)), "..");
    }
    {
        auto foo = List<float>{};
        foo.push(1.1f);
        foo.push(2.2f);

        auto bar = mempty(List<float>{});
        auto abofa = [](auto a, auto b){return associative_binary_operation_for_alternative(a, b);};
        EXPECT_EQ(foo, (abofa(foo, bar)), "..");
        EXPECT_EQ(foo, (abofa(bar, foo)), "..");
        EXPECT_EQ(bar, (abofa(bar, bar)), "..");
    }
    {
        auto foo = List<unsigned int>{};
        for (size_t i{1}; i<10; ++i) {
            foo.push(i);
        }
        auto odd = [](auto i){return (i % 2) == 0;};
        auto bar = List<unsigned int>{};
        bar.push(2);
        bar.push(4);
        bar.push(6);
        bar.push(8);
        EXPECT_EQ(bar, (filter(odd, foo)), "..");
    }

    print_summary();
    return failed;
}
