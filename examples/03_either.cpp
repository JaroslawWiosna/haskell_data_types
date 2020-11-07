#include "../haskell_data_types.hpp"

#include <iostream>
#include <string>

int main() {
    auto foo = Either<int, std::string>{true, {}, "This is foo!"};
    {
        foo.r += " Welcome everybody!";

        std::cout << "auto foo = Either<int, std::string>"
                  <<"{true, {}, \"This is foo!\"};" << '\n';
        std::cout << "foo.r += \" Welcome everybody!\";" << '\n';
        // std::cout << "foo.r == " << foo.r << '\n';

    }
}
