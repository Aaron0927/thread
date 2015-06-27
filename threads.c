#include <malloc.h>
#include "threads.h"

struct thread *thread_create(void (*f)(void *arg), void *arg) {
	// get memory
	struct thread *t = malloc(sizeof(struct thread));
	void *stack = memalign(8, STACK_SIZE);
	t->stack_pointer = stack;
	// set pointers
	t->esp = t->ebp = stack + STACK_SIZE;
	t->function = f;
	t->argument = arg;
	t->run = FALSE;
	return t;
}

void thread_add_runqueue(struct thread *t) {
	struct node *conductor = malloc(sizeof(struct node));
	conductor->t = t;
	// if nothing is in the queue, start queue
	if (root == NULL) {
		conductor->previous = conductor;	// not entirely sure if conductor
		conductor->next = conductor;
		root = conductor;
		last = conductor;
	}
	// if queue exists, add to queue
	else {
		last->next = conductor;
		conductor->previous = last;
		// link to root
		conductor->next = root;
		root->previous = conductor;
		last = conductor;
	}
}

void schedule(void) {
	if(current == NULL)
		current = root;
	else 
		next_node = current->next;
	
}

void thread_exit(void)
{
	exited = TRUE;
	Boolean fullExit = FALSE;
	if(current == root)
		root = current->next;
	if(current == last)
		last = current->previous;
		
	// unlinking
	current->previous->next = current->next;
	current->next->previous = current->previous;
	// go to next thread
	next_node = current->next;
	if(current == next_node)
		fullExit = TRUE;
	// free thread memory
	free(current->t->stack_pointer);
	free(current->t);
	free(current);
	//!!!!!!!!!! need to free the node as well !!!!!!!!!!!!
	if(fullExit == TRUE)
		longjmp(ret->buf, 1);
	
	dispatch();
}

void thread_yield(void) {
	// current thread
	if(!setjmp(current->t->buf)) {
		schedule();
		dispatch();
	}
}

void dispatch(void) {
	void (*foo)();
	// if previous thread exited, don't save it's state
	if(exited) {
		exited = FALSE;
		// check if current has been run or not
		// next thread hasn't been		
		if (next_node->t->run == FALSE)
		{
			next_node->t->run = TRUE;

			// restore esp, ebp for next thread
			__asm __volatile("mov %%rax, %%rsp" : : "a" (next_node->t->esp) );
			__asm __volatile("mov %%rax, %%rbp" : : "a" (next_node->t->ebp) );
			current = next_node;
			foo = current->t->function;
			foo(current->t->argument);
		}
	
		// both threads have been run
		else
		{
			// restore esp, ebp for next thread
			__asm __volatile("mov %%rax, %%rsp" : : "a" (next_node->t->esp) );
			__asm __volatile("mov %%rax, %%rbp" : : "a" (next_node->t->ebp) );
			current = next_node;
			longjmp(current->t->buf, 1);
		}

	}
	else {
		// case 1: when we the first thread ever run
		if (next_node == NULL)
		{
			current->t->run = TRUE;
			// restore esp, ebp for current thread
			__asm __volatile("mov %%rax, %%rsp" : : "a" (current->t->esp) );
			__asm __volatile("mov %%rax, %%rbp" : : "a" (current->t->ebp) );
			foo = current->t->function;
			foo(current->t->argument);
		}

		// case 2: next thread needs to be run
		else if (next_node->t->run == FALSE)
		{
			next_node->t->run = TRUE;
			// save esp, ebp for current thread
			__asm __volatile("mov %%rsp, %%rax" : "=a" (current->t->esp) : );
			__asm __volatile("mov %%rbp, %%rax" : "=a" (current->t->ebp) : );

			// restore esp, ebp for next thread
			__asm __volatile("mov %%rax, %%rsp" : : "a" (next_node->t->esp) );
			__asm __volatile("mov %%rax, %%rbp" : : "a" (next_node->t->ebp) );

			current = next_node;
			foo = current->t->function;
			foo(current->t->argument);
		}
	
		// case 3: both threads have been run
		else
		{
			// save esp, ebp for current thread
			__asm __volatile("mov %%rsp, %%rax" : "=a" (current->t->esp) : );
			__asm __volatile("mov %%rbp, %%rax" : "=a" (current->t->ebp) : );

			// restore esp, ebp for next thread
			__asm __volatile("mov %%rax, %%rsp" : : "a" (next_node->t->esp) );
			__asm __volatile("mov %%rax, %%rbp" : : "a" (next_node->t->ebp) );
			current = next_node;
			longjmp(current->t->buf, 1);
		}
	}
	thread_exit();
	return;
}

void thread_start_threading(void)
{
	// meant for returning
	ret = thread_create(NULL, NULL);
	if(!setjmp(ret->buf)) {
		schedule();
		dispatch();
	}

}


