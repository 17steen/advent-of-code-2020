CXX := g++-10 -m64 -std=gnu++2a -Wall -Wextra -g3 -O3 -I../includes -fconcepts -fconcepts-diagnostics-depth=5

OBJ := main.o

day : main.o
	$(CXX) -o day $(OBJ)

main.o : main.cpp

.PHONY : clean

clean : 
	rm *.o

