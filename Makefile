CC=gcc 
CFLAGS=-Wall

all: project
project: project.o
project.o: project.c

clean:
	rm -f project project.o
run: project
	./project < in