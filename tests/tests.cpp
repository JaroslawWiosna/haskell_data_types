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
void expect_eq(Expected e, Actual a, std::string desc, const char *file, int line) {
    if (e == a) {
        ++passed;
    } else {
        ++failed;
        std::cout << file << ':' << line << ' ' << desc << "\n";
    }
}

template <typename Expected, typename Actual> 
void expect_neq(Expected e, Actual a, std::string desc, const char *file, int line) {
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

        EXPECT_EQ(foo, bar, std::string{"Two different instances of zero initialized maybe should be equal"});
    }
    {
        auto foo = Maybe<int>{};
        auto bar = Maybe<float>{};

        EXPECT_EQ(foo, bar, std::string{"Two different instances of zero initialized maybe should be equal"});
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
    print_summary();
    return failed;
}
