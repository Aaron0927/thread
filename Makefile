CFLAGS = -O0 -ggdb3 -Wall
all: threader

threader:main.o threads.o
	gcc $(CFLAGS) threads.o main.o -o threader
threads.o: threads.c threads.h
	gcc $(CFLAGS) threads.c -c

main.o: main.c threads.h
	gcc $(CFLAGS) main.c -c 

.PHONY : clean
clean :
	rm threader *.o
