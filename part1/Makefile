CC=gcc 
CFLAGS= -g -Wall

all: clean project run

project.o: project.c
	$(CC) -c $(CFLAGS) $<

project: project.o
	$(CC) -o project project.o

	
run: project
	./project < in
	
clean:
	rm -f project project.o