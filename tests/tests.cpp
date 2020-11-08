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

template <typename T>
bool is_printable(T t) { return false; }

template <>
bool is_printable(int t) { return true; }

template <typename Expected, typename Actual> 
void expect_eq(Expected e, Actual a, std::string desc, const char *file, int line) {
    ((e == a) ? ++passed : ++failed);
    if (e == a) {
        ++passed;
    } else {
        ++failed;
        std::cout << file << ':' << line << ' ' << desc << "\n";
    }
}

#define EXPECT_EQ(val1, val2, val3) \
    expect_eq(val1, val2, val3, __FILE__, __LINE__);

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
        auto foo = Maybe<int>{};
        auto bar = Maybe<float>{};

        EXPECT_EQ(foo, bar, std::string{"Two different instances of zero initialized maybe are not equal"});
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
