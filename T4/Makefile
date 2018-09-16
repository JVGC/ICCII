objects = main.o Bubble.o process.o circular.o

all: $(objects)
	gcc -o main $(objects) -g -I.

main.o: main.c Bubble.h process.h circular.h
	gcc -c main.c

circular.o: circular.c circular.h process.h
	gcc -c circular.c

process.o: process.c process.h
	gcc -c process.c

Bubble.o: Bubble.c Bubble.h process.h
	gcc -c Bubble.c	
clean:
	rm *.o main


run:
	./main
