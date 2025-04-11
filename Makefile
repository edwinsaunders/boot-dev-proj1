all: main

main.o: main.c
	gcc -c main.c -o main.o

main: main.o
	gcc -o main main.o -lncurses -lm

clean:
	rm -f main.o main
