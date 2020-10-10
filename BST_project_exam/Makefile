EXE = bst_test
CXX = c++
CXXFLAGS = -I include -g -std=c++14 -Wall -Wextra -Wreorder

SRC = main.cpp bst.cpp
OBJ = $(SRC:.cpp=.o)
INC = include/bst.hpp  include/iterator.hpp 

all: $(EXE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE): main.o bst.o
	$(CXX) $^ -o $(EXE) 

main.o: include/bst.hpp include/iterator.hpp

bst.o:	include/bst.hpp	include/iterator.hpp

format: $(SRC) $(INC)
	@clang-format -i $^ -verbose || echo "Please install clang-format to run this commands"


clean:
	rm -rf src/*.o *.o $(EXE)  */*~ *~ a.out*
