CXX=g++
CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -I.

SRC=Inventory.cpp
HEADERS=DynArray.hpp HashMap.hpp Product.hpp CSVReader.hpp Inventory.hpp

all: amazon_repl tests

amazon_repl: main.cpp $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(SRC)

tests: test_containers.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ test_containers.cpp

clean:
	rm -f amazon_repl tests
