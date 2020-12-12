CXX := g++-10 -pthread -m64 -Wall -Wextra -std=c++20 -g3 -O3

.PHONY : clean

all : day

day : main.o 
	$(CXX) main.o -o day

main.o : main.h

clean :
	rm *.o