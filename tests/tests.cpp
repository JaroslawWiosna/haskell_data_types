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

int main() {
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
    {
        auto foo = Either<int, double>{true, {}, 42.0};
        foo.r += 1;
        auto bar = fmap([](double a){return 2.0 * a;}, foo);

        EXPECT_EQ(86.0, bar.value(), {});
    }
    {
        auto foo = Either<int, double>{false, 2, {}};
        foo.r += 1;
        auto bar = fmap([](double a){return 2.0 * a;}, foo);

        EXPECT_EQ(2, bar.value(), {});
    }
    print_summary();
    return failed;
}
