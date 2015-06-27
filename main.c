#include <stdio.h>
#include "threads.h"

void f3(void *arg) {
	int i; 
	for(i = 0; i < 100; i++) {
		printf("in f3\n");
		thread_yield();
	}
}

void f2(void *arg) {
	int i = 20;
	int total = 0;
	for(i = 20; i > 0; i--)  
		total+= i;
	printf("total = %i\n", total);
	
}

void f1(void *arg)
{
    int i = 0;
    struct thread *t1 = thread_create(f2, NULL);
    struct thread *t2 = thread_create(f3, NULL);
    thread_add_runqueue(t1);
    thread_add_runqueue(t2);
    while(i < 100) {
        printf("thread 1: %d\n", i++);
        thread_yield();
    }
}

int main(int argc, char **argv)
{
    struct thread *t1 = thread_create(f1, NULL);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\nexited\n");
    return 0;
}

