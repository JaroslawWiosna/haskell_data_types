CXX_FLAGS=-ggdb -std=c++20 -fconcepts-diagnostics-depth=3
INC?=
LIB?=
CXX=c++
CC=cc

.PHONY: all
all: hello_world \
	maybe \
	either \
	eq \
	monoid

hello_world: 01_hello_world.cpp ../haskell_data_types.hpp
	$(CXX) $(CXX_FLAGS) $(INC) 01_hello_world.cpp -o hello_world $(LIB)

maybe: 02_maybe.cpp ../haskell_data_types.hpp
	$(CXX) $(CXX_FLAGS) $(INC) 02_maybe.cpp -o maybe $(LIB)

either: 03_either.cpp ../haskell_data_types.hpp
	$(CXX) $(CXX_FLAGS) $(INC) 03_either.cpp -o either $(LIB)

eq: 04_eq.cpp ../haskell_data_types.hpp
	$(CXX) $(CXX_FLAGS) $(INC) 04_eq.cpp -o eq $(LIB)

monoid: 05_monoid.cpp ../haskell_data_types.hpp
	$(CXX) $(CXX_FLAGS) $(INC) 05_monoid.cpp -o monoid $(LIB)

.PHONY: clean
clean:
	rm -f hello_world
	rm -f maybe
	rm -f either
	rm -f eq
	rm -f monoid

