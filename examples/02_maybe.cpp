#include "../haskell_data_types.hpp"

#include <cassert>
#include <iostream>
#include <optional>

using namespace haskell_data_types;

template<typename T>
bool f(T a, T b, T c) requires Eq<T> {
    return {};
}

int main() {
    {
        auto foo = Maybe<int>{true, 42};

        std::cout << "auto foo = Maybe<int>{true, 42};" << '\n';
        std::cout << "foo.has_value == " << foo.has_value << '\n';
        std::cout << "foo.value == " << foo.value << '\n';
        std::cout << '\n';
    }
    {
        auto foo = Maybe<int>{};

        std::cout << "auto foo = Maybe<int>{};" << '\n';
        std::cout << "foo.has_value == " << foo.has_value << '\n';
        std::cout << "foo.value == " << foo.value << '\n';
        std::cout << '\n';
    }
    {
        auto foo = Maybe<float>{};

        std::cout << "auto foo = Maybe<float>{};" << '\n';
        std::cout << "foo.has_value == " << foo.has_value << '\n';
        std::cout << "foo.value == " << foo.value << '\n';
        std::cout << '\n';
    }
    {
        auto foo = Maybe<int>{true, 42};
        auto bar = fmap([](int a){return a+1;}, foo);

        std::cout << "auto foo = Maybe<int>{true, 42};" << '\n';
        std::cout << "auto bar = fmap([](int a){return a+1;}, foo);" << '\n';
        std::cout << "bar.has_value == " << bar.has_value << '\n';
        std::cout << "bar.value == " << bar.value << '\n';
        std::cout << '\n';
        
        assert(43 == bar.value);
    }
    {
        auto bar = Maybe<int>{true, 20};
        auto bay = Maybe<int>{true, 400};
        auto baz = Maybe<int>{};

        bar == bay;
        bar == baz;
        bay == baz;
        bar != bay;
        bar != baz;
        bay != baz;
        f(bar, bay, baz);
    }
    {
        auto foo = Maybe<std::string>{true, "lorem ipsum"};
        auto bar = fmap([](std::string s){return s.size();}, foo);

        assert(11 == bar.value);
    }
    {
        {
            auto foo = std::optional<std::string>{"hello world"};
            auto bar = std::optional<std::string>{std::nullopt};

            assert(std::string{"hello world"} == foo.value_or(std::string{"NOTHING"}));
            assert(std::string{"NOTHING"}     == bar.value_or(std::string{"NOTHING"}));

            // assert(11 == fmap([](std::string s){return s.size();}, foo).value_or{"NOTHING"});
            // assert(0  == fmap([](std::string s){return s.size();}, bar).value_or{"NOTHING"});

            assert(11 == (foo.has_value() ? foo.value().size() : 0)); // tedious because you have to type foo twice
            assert(0  == (bar.has_value() ? bar.value().size() : 0)); // tedious because you have to type bar twice

            assert(11 == foo.value_or(std::string{}).size()); // tedious because you have to know what value of std::string return 0 on size() the upfront
            assert(0  == bar.value_or(std::string{}).size()); // tedious because you have to know what value of std::string return 0 on size() the upfront
        }
        {
            auto foo = Maybe<std::string>{true, std::string{"hello world"}};
            auto bar = Maybe<std::string>{};

            assert(std::string{"fmapped"} == fmap([](std::string s){return std::string{"fmapped"};}, foo).value_or(std::string{"NOTHING"}));
            assert(std::string{"NOTHING"} == fmap([](std::string s){return std::string{"fmapped"};}, bar).value_or(std::string{"NOTHING"}));

            //assert(static_cast<int>(11) == (int)maybe(0, [](std::string s){return s.size();}, foo));
            //assert(static_cast<int>(0)  == (int)maybe(0, [](std::string s){return s.size();}, bar));
        }
    }
}
