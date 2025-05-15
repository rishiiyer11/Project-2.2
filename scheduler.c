// Rishi Iyer 921141938
#include <stdlib.h>
#include <ucontext.h>

#include "scheduler.h"

/*
Resources:
Lin Man
https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/
https://www.w3schools.com/c/ref_stdlib_malloc.php
https://www.geeksforgeeks.org/free-function-in-c-library-with-examples/
*/

// create static globals for main and task contexts
static ucontext_t mc;
static ucontext_t tc[TASK_COUNT_MAX];
// arr to hold stack mem
static char *stackArr[TASK_COUNT_MAX];

int scheduler_run(void (*task_func)(int), int task_cnt, const int *task_sched)
{
	/* Check for errors: <1 tasks, >TASK_COUNT_MAX, !task_func, !task_sched, malloc err (alr. allocated) */
	if(task_cnt < 1 || task_cnt > TASK_COUNT_MAX || !task_func || !task_sched){
		// failure
		return -1;
	}

	// task context 
	for(int i = 0; i < task_cnt; i++){
		// allocate mem for each stack
		stackArr[i] = malloc(STACK_SIZE);

		// if malloc alr. allocated it returns NULL
		if(stackArr[i] == NULL){
			// if fail free all mem
			for(int x = 0; x < i; x++){
				free(stackArr[x]);
			}
			return -1;
		}

		// set context
		if (getcontext(&tc[i]) < 0){
			// if fail free all mem
			for(int x = 0; x < i; x++){
				free(stackArr[x]);
			}
			return -1;
		}

		// set context for stack
		tc[i].uc_link = &mc;
		tc[i].uc_stack.ss_sp = stackArr[i];
		tc[i].uc_stack.ss_size = STACK_SIZE;

		// make context
		makecontext(&tc[i], (void (*)(void))task_func, 1, i);
	}

	// main context
	if (getcontext(&mc) < 0){
		// if fail free all mem
		for(int i = 0; i < task_cnt; i++){
			free(stackArr[i]);
		}
		return -1;
	}

	// run
	int count = 0;
	while(1){
		int task = task_sched[count];
		count++;

		// sched end
		if(task == -1){
			break;
		}

		// invalid task
		if(task >= task_cnt || task < 0){
			// if fail free all mem
			for(int i = 0; i < task_cnt; i++){
				free(stackArr[i]);
			}
			return -1;
		}

		// swap
		if(swapcontext(&mc, &tc[task]) < 0){
			// if fail free all mem
			for(int i = 0; i < task_cnt; i++){
				free(stackArr[i]);
			}
			return -1;
		}

	}

	// free all mem
	for(int i = 0; i < task_cnt; i++){
		free(stackArr[i]);
	}

	// success
	return 0;
}

void scheduler_yield(int task_id)
{
	// error check b4 swap
	if(task_id >= 0 && task_id < TASK_COUNT_MAX ){
		swapcontext(&tc[task_id], &mc);
	}
	
}
