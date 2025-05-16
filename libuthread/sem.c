#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "queue.h"
#include "private.h"
#include "sem.h"

struct semaphore {
	/* TODO Phase 3 */
	int count;
	queue_t wait_queue;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	struct semaphore *sem = malloc(sizeof(*sem));
	if (!sem) {
		return NULL;
	}
	sem->count = (int)count;
	sem->wait_queue = queue_create();
	if (!sem->wait_queue) {
		free(sem);
		return NULL;
	}
	return sem;
}

int sem_destroy(sem_t sem)
{
    assert(sem != NULL);

    preempt_disable();

    /* check if queue empty */
    if (queue_length(sem->wait_queue) > 0) {
        preempt_enable();
        return -1;
    }
    
    queue_destroy(sem->wait_queue);
    free(sem);

    preempt_enable();
    
    return 0;
}

int sem_down(sem_t sem)
{
    assert(sem != NULL);

    preempt_disable();

    sem->count--;
    if (sem->count < 0) {
        /* block until we really have the resource */
        queue_enqueue(sem->wait_queue, uthread_current());
        uthread_block();
    }

    preempt_enable();

    return 0;
}

int sem_up(sem_t sem)
{
    assert(sem != NULL);
    
    preempt_disable();

    sem->count++;
    if (sem->count <= 0) {
        /* wake exactly one waiter */
        void *next;
        if (queue_dequeue(sem->wait_queue, &next) == 0) {
            uthread_unblock((struct uthread_tcb*)next);
        }
    }

    preempt_enable();

    return 0;
}
