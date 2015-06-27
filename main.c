#include <stdio.h>
#include "threads.h"

void f3(void *arg) {
	int i; 
	for(i = 0; i < 10; i++) {
		printf("in f3\n");
		thread_yield();
	}
}

void f2(void *arg) {
	int i = 20;
	int total = 0;
	for(i = 20; i > 0; i--)  
		total+= i;
	printf("in f2 total = %i\n", total);
	
}

void f4(void *arg) {
    int i;
    for(i = 0; i < 5; ++i) {
        printf("in f4, i = %d\n", i);
        thread_yield();
    }
}

void f1(void *arg)
{
    int i = 0;
    struct thread *t1 = thread_create(f2, NULL);
    struct thread *t2 = thread_create(f3, NULL);
    struct thread *t3 = thread_create(f4, NULL);
    thread_add_runqueue(t1);
    thread_add_runqueue(t2);
    thread_add_runqueue(t3);
    while(i < 10) {
        printf("in f1 : %d\n", i++);
        thread_yield(); // 进入待执行状态
    }
}

int main(int argc, char **argv)
{
    struct thread *t1 = thread_create(f1, NULL);
    thread_add_runqueue(t1);
    thread_start_threading(); // 线程启动
    printf("\nin main exited\n");
    return 0;
}

