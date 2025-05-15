#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

struct uthread_tcb {
	ucontext_t conetext;
	void *stack;
};

static queue_t *ready_queue = NULL;
static uncontext_t schedulter_ctx;
static struct uthread_tcb *current_thread = NULL;

static void thread_start(uthread_func_t, void *arg) {
	func(arg);
	uthread_exit();
}
struct uthread_tcb *uthread_current(void)
{
	return current_thread
}

void uthread_yield(void)
{
	queue_enqueue(ready_queue, current_thread);
	swapcontext(&current_thread->conetext, &schedulter_ctx);
}

void uthread_exit(void)
{
	free(current_thread->stack);
	free(current_thread);
	current_thread = NULL;
	setcontext(&schedulter_ctx);
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *tcb - malloc(sizeof(*tcb));
	if (tcb == NULL) {
		return -1;
	}

	if(getcontext(&tcb->conetext) < 0) {
		free(tcb);
		return -1;
	}

	tcb->stack = malloc(UTHREAD_STACK_SIZE);
	if (tcb->stack) {
		free(tcb);
		return -1;
	}
	
	tcb->context.uc_stack.ss_sp = tcb->stack;
    tcb->context.uc_stack.ss_size = UTHREAD_STACK_SIZE;
    /* when this thread finishes, it returns to the scheduler */
    tcb->context.uc_link = &scheduler_ctx;

    /* set it to start in our wrapper that calls func(arg) */
    makecontext(&tcb->context,
                (void (*)(void))thread_start,
                2,
                func,
                arg);

    /* enqueue for round‐robin scheduling */
    queue_enqueue(ready_queue, tcb);
    return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	void(preempt);

	/* make run_queue */
	ready_queue = queue_create();
	if (ready_queue == NULL) {
		return -1;
	}

	/* create first thread */
	if (uthread_create(func, arg) < 0) {
		queue_destroy(ready_queue);
		return -1;
	}

	/* scheduler loop w/ dequeue, run, repeat */
	while (!queue_empty(ready_queue)) {
		struct uthread_tcb *next_in_queue = queue_dequeue(ready_queue);
		current_thread = next;
		swarpcontext(&scheduler_ctx, &next->context);
	}

	/* clean up */
	queue_destroy(ready_queue);
	ready_queue = NULL;
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */

}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
}