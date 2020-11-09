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
    expect_eq(val1, val2, val3, __FILE__, __LINE__);

#define EXPECT_NEQ(val1, val2, val3) \
    expect_neq(val1, val2, val3, __FILE__, __LINE__);

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
        EXPECT_EQ((Maybe<int>{})        , liftA2(add, foo, baz), "liftA2");
        EXPECT_EQ((Maybe<int>{})        , liftA2(add, bar, baz), "liftA2");
        EXPECT_EQ((Maybe<int>{})        , liftA2(add, bax, baz), "liftA2");
    }
    // MAYBE WITH LAMBDA INSIDE AS A PARAMETER
    {
        auto foo = [](int a, int b){return a + b + 1;};
        auto bar = Maybe{true, foo};
    }
    print_summary();
    return failed;
}
