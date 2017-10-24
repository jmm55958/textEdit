all: 1730ed

1730ed: 1730ed.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -lncurses -lmenu -o 1730ed 1730ed.o

1730ed.o: 1730ed.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c -lncurses -lmenu 1730ed.cpp

clean:
	rm -f *.o
	rm -f 1730ed
