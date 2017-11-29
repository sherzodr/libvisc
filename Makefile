all: main

clean:
	$(RM) main.o libvisc.o main

main: main.o libvisc.o
	g++ -O -Wall -o main main.o libvisc.o
	
main.o: libvisc.o main.cpp
	g++ -O -Wall -o main.o -c main.cpp

libvisc.o: libvisc.cpp libvisc.h
	g++ -O -Wall -o libvisc.o -c libvisc.cpp

touch: 
	touch *.h *.cpp

