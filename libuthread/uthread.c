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
#include "preempt.c"

#define UTHREAD_STACK_SIZE (64 * 1024)

struct uthread_tcb {
	ucontext_t context;
	void *stack;
};

static queue_t ready_queue = NULL;
static ucontext_t scheduler_ctx;
static struct uthread_tcb *current_thread = NULL;

static void thread_start(uthread_func_t func, void *arg) {
    preempt_enable();
	func(arg);
	uthread_exit();
}
struct uthread_tcb *uthread_current(void) {
	return current_thread;
}

void uthread_yield(void)
{
    preempt_disable();
	queue_enqueue(ready_queue, current_thread);
	swapcontext(&current_thread->context, &scheduler_ctx);
}

void uthread_exit(void)
{
    preempt_disable();
	free(current_thread->stack);
	free(current_thread);
	current_thread = NULL;
	setcontext(&scheduler_ctx);
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *tcb = malloc(sizeof(*tcb));
	if (tcb == NULL) {
		return -1;
	}

	if(getcontext(&tcb->context) < 0) {
		free(tcb);
		return -1;
	}

    tcb->stack = malloc(UTHREAD_STACK_SIZE);
	if (!tcb->stack) {
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
	preempt_start(preempt);
    ready_queue = queue_create();
    if (!ready_queue) return -1;

    if (uthread_create(func, arg) < 0) {
        queue_destroy(ready_queue);
        return -1;
    }

    /* KEEP RUNNING WHILE THERE ARE THREADS IN THE QUEUE */
    while (queue_length(ready_queue) > 0) {   // ← was “while (!queue_length(...))”
        struct uthread_tcb *next = NULL;
        queue_dequeue(ready_queue, (void**)&next);
        current_thread = next;
        if (preempt) preempt_enable();
        swapcontext(&scheduler_ctx, &next->context);
        if (preempt) preempt_disable();
    }

    preempt_stop();
    ready_queue = NULL;
    return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
    preempt_disable();
    swapcontext(&current_thread->context, &scheduler_ctx);
    preempt_enable();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
    preempt_disable();
    queue_enqueue(ready_queue, uthread);
    preempt_enable();
}