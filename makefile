CC=gcc
LIBS=-lncurses

main: main.c tilemap.c being.c
	$(CC) main.c tilemap.c being.c -o main $(LIBS)