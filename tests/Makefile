CXX_FLAGS=-ggdb -std=c++20 -fconcepts -fconcepts-diagnostics-depth=3 --coverage
INC?=
LIB?=
CXX=c++
CC=cc
GCOV?=

.PHONY: all run
all: tests

tests: tests.cpp ../haskell_data_types.hpp
	$(CXX) $(CXX_FLAGS) $(INC) tests.cpp -o tests $(LIB)

run: tests
	./tests
	$(GCOV) --version
	$(GCOV) tests.cpp ../haskell_data_types.hpp -m

.PHONY: clean
clean:
	rm -f tests

