
DEBUG_FLAGS := -DDEBUG -Wshadow -pedantic -g -fsanitize=address -fsanitize-address-use-after-scope

RELEASE_FLAGS := -g3 -O3

CXXFLAGS := -m64 -std=gnu++2a -Wall -Wextra -I../includes -fconcepts -fconcepts-diagnostics-depth=5

CXX := g++-10 



OBJ := main.o

release: CXXFLAGS += $(RELEASE_FLAGS)
release: day


debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: day

day:  
day: main.o
	$(CXX) $(CXXFLAGS) -o day $(OBJ) 



main.o : main.cpp

.PHONY : clean

clean : 
	rm *.o

