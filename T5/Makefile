objects = main.o terminal.o bubble.o

all: $(objects)
	gcc -o main $(objects) -g -lm -I.

main.o: main.c terminal.h bubble.h
	gcc -c main.c

terminal.o: terminal.c terminal.h
	gcc -c terminal.c
bubble.o: bubble.c bubble.h terminal.h
	gcc -c bubble.c	

clean:
	rm *.o main


run:
	./main	