objects = main.o lexical.o

all: $(objects)
	gcc -o main $(objects) -g -I.

main.o: main.c lexical.h
	gcc -c main.c

lexical.o: lexical.c lexical.h
	gcc -c lexical.c

clean:
	rm *.o main


run:
	./main 
