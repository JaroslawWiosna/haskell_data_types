CXX_FLAGS=-ggdb -std=c++2a -fconcepts-diagnostics-depth=3

.PHONY: all
all: 01_hello_world 02_maybe

01_hello_world: 01_hello_world.cpp
	$(CXX) $(CXX_FLAGS) 01_hello_world.cpp -o hello_world

02_maybe: 02_maybe.cpp
	$(CXX) $(CXX_FLAGS) 02_maybe.cpp -o maybe



.PHONY: clean
clean:
	rm -f hello_world
	rm -f maybe

