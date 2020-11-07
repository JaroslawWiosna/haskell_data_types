CXX_FLAGS=-ggdb -std=c++20 -fconcepts-diagnostics-depth=3
INC?=
LIB?=
CXX=c++
CC=cc

.PHONY: all
all: 01_hello_world \
	02_maybe \
	03_either

01_hello_world: 01_hello_world.cpp
	$(CXX) $(CXX_FLAGS) $(INC) 01_hello_world.cpp -o hello_world $(LIB)

02_maybe: 02_maybe.cpp
	$(CXX) $(CXX_FLAGS) $(INC) 02_maybe.cpp -o maybe $(LIB)

03_either: 03_either.cpp
	$(CXX) $(CXX_FLAGS) $(INC) 03_either.cpp -o either $(LIB)

.PHONY: clean
clean:
	rm -f hello_world
	rm -f maybe
	rm -f either

