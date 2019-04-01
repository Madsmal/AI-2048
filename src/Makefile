CC=gcc
CFLAGS=-I
binary=main

all: clean $(binary)
clean:
	rm -f $(binary) *.o
main: main.c game.c
	$(CC) -o main main.c game.c
