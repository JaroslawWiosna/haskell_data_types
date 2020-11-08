CXX_FLAGS=-ggdb -std=c++20 -fconcepts -fconcepts-diagnostics-depth=3
INC?=
LIB?=
CXX=c++
CC=cc

.PHONY: all
all: tests

tests: tests.cpp ../haskell_data_types.hpp
	$(CXX) $(CXX_FLAGS) $(INC) tests.cpp -o tests $(LIB)

.PHONY: clean
clean:
	rm -f tests

