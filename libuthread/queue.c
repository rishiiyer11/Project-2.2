#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
/*
Resoureces
https://www.geeksforgeeks.org/queue-data-structure/#
https://www.geeksforgeeks.org/queue-in-c/
https://www.geeksforgeeks.org/c-program-to-implement-circular-queue/
*/
struct queue {
	/* TODO Phase 1 */
	void **buf; // arr of ptrs
	int head; // index at front
	int tail; // insert at
	int size; // num ele
	int cap; // total buf cap

};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t q = malloc(sizeof(struct queue));
	
	if (q == NULL){
		return NULL;
	}
	
	q->buf = malloc(32 * sizeof(void*)); // allocate for init buf (just defaulting to 32)

	if (q->buf == NULL){
		free(q);
		return NULL;
	}

	q->head = 0;
	q->tail = 0;
	q->size = 0;
	q->cap = 32;

	return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */

	if(queue == NULL){
		return -1;
	}

	if(queue->size > 0){
		return -1;
	}

	free(queue->buf);

	free(queue);
	
	return 0;

}

// add resize function because we are using an array (static size)
static int q_resize(queue_t q, int capNew){
	void **bufNew = malloc(capNew * sizeof(void*));
	if (bufNew == NULL){
        return -1;
	}

	// copy ele to re-establish buf
    for (int i = 0; i < q->size; i++) {

		// ensure proper index in circular q
        int index = (q->head + i) % q->cap;
        bufNew[i] = q->buf[index];
    }
    
    free(q->buf);
    q->buf = bufNew;
    q->head = 0;
    q->tail = q->size;
    q->cap = capNew;
    
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if (queue == NULL || data == NULL){
        return -1;
    }

    if (queue->size == queue->cap) {
        if (q_resize(queue, queue->cap * 2) == -1)
            return -1;
    }
    
    queue->buf[queue->tail] = data;
    queue->tail = (queue->tail + 1) % queue->cap;
    queue->size++;
    
    return 0;


}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if (queue == NULL || data == NULL){
        return -1;
    }

    // check empty
    if (queue->size == 0){
        return -1;
    }

    // get head
    *data = queue->buf[queue->head];
    queue->head = (queue->head + 1) % queue->cap;
    queue->size--;
    
    /* resize down (not sure if neccessary)
    if (queue->size > 0 && queue->size < queue->cap / 4 && queue->cap > 32) {
        q_resize(queue, queue->cap / 2);
    }
	*/
    
    return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
    if (queue == NULL || data == NULL){
        return -1;
    }

    // check empty
    if (queue->size == 0){
        return -1;
    }
    // find ele
    int found = -1;
    for (int i = 0; i < queue->size; i++) {
        int index = (queue->head + i) % queue->cap;
        if (queue->buf[index] == data) {
            found = i;
            break;
        }
    }
    
    // didn't find
    if (found == -1){
        return -1;
	}

    // remove ele
    for (int i = found; i < queue->size - 1; i++) {
        int current = (queue->head + i) % queue->cap;
        int next = (queue->head + i + 1) % queue->cap;
        queue->buf[current] = queue->buf[next];
    }
    
    /* Update tail position */
    queue->tail = (queue->tail - 1 + queue->cap) % queue->cap;
    queue->size--;
    
    /* resize down (not sure if neccessary)
    if (queue->size > 0 && queue->size < queue->cap / 4 && queue->cap > 32) {
        q_resize(queue, queue->cap / 2);
    }
	*/
    
    return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	if (queue == NULL || func == NULL){
		return -1;
	}

	// copy to iterate thru
	void **items = malloc(queue->size * sizeof(void*));
	if (items == NULL){
		return -1;
	}

	// copy ele to buf
	for (int i = 0; i < queue->size; i++) {
		int index = (queue->head + i) % queue->cap;
		items[i] = queue->buf[index];
	}

	// iterate
	int ogSize = queue->size;
	for (int i = 0; i < ogSize; i++) {
		func(queue, items[i]);
	}

	free(items);
	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if (queue == NULL){
		return -1;
	}

	return queue->size;
}

