EXE = bst_test
CXX = g++
CXXFLAGS = -I include -std=c++14 -Wall -Wextra

INC = include/bst.hpp  include/iterator.hpp 

all: $(EXE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE): main.o
	$(CXX) $^ -o $(EXE) 

main.o: include/bst.hpp include/iterator.hpp

clean:
	rm -rf src/*.o *.o $(EXE)  */*~ *~ a.out*
