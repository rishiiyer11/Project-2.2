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
	/* TODO Phase 3 */
	assert(sem != NULL);
    /* no threads should still be waiting */
    assert(queue_length(sem->wait_queue));
    queue_destroy(sem->wait_queue);
    free(sem);
    return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	assert(sem != NULL);
    sem->count--;
    if (sem->count < 0) {
        /* blocks the current thread */
        queue_enqueue(sem->wait_queue, uthread_current());
        uthread_block();
    }
    return 0;
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	assert(sem != NULL);
    sem->count++;
    if (sem->count <= 0) {
        /* wake one waiting thread */
        struct uthread_tcb *next = NULL;
        queue_dequeue(sem->wait_queue, (void**)&next);
        uthread_unblock(next);
    }
    return 0;
}

