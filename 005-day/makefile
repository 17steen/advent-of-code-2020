CXX := g++-10 -m64 -std=gnu++2a -Wall -Wextra -g -I../includes -fconcepts 

OBJ := main.o

day : main.o
	$(CXX) -o day $(OBJ)

main.o : main.cpp

.PHONY : clean

clean : 
	rm *.o

