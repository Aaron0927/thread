#ifndef THREADS_H_
#define THREADS_H_
#include <setjmp.h>
#define STACK_SIZE 4096 

typedef short Boolean;
#define TRUE 1
#define FALSE 0

Boolean exited;

struct thread {
	void *esp;
	void *ebp;
	void (*function)(void *arg);
	void *argument;
	void *stack_pointer;
	jmp_buf buf;
	Boolean run;
};

struct node {
	struct thread *t;
	struct node *next;
	struct node *previous;
};

struct node *root;
struct node *last;
struct node *current;
struct node *next_node;

struct thread *ret;

struct thread *thread_create(void (*f)(void *arg), void *arg);
void thread_add_runqueue(struct thread *t);
void thread_yield(void);
void dispatch(void);
void schedule(void);
void thread_exit(void);
void thread_start_threading(void);

#endif // THREADS_H
