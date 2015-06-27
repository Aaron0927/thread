CFLAGS = -O0 -ggdb3 -Wall
all: thread

thread : main.o threads.o
	gcc $(CFLAGS) threads.o main.o -o thread
threads.o : threads.h
	gcc $(CFLAGS) threads.c -c

main.o : threads.h
	gcc $(CFLAGS) main.c -c 

.PHONY : clean
clean :
	rm thread *.o
