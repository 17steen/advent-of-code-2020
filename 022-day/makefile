
DEBUG_FLAGS := -DDEBUG -Wshadow -pedantic -g -Og -fsanitize=address -fsanitize-address-use-after-scope

RELEASE_FLAGS :=  -O3

CXXFLAGS := -m64 -std=gnu++2a -Wall -Wextra -I../includes -fconcepts -fconcepts-diagnostics-depth=5

CXX := g++-10 



OBJ := main.o

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: day

release: CXXFLAGS += $(RELEASE_FLAGS)
release: day

day:  
day: main.o
	$(CXX) $(CXXFLAGS) -o day $(OBJ) 



main.o : main.cpp

.PHONY : clean

clean : 
	rm *.o

