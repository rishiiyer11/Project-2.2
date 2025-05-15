#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*
Resources:
queue_tester_example.c
https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1188/lectures/Lecture6/Lecture6.pdf
https://www.geeksforgeeks.org/queue-data-structure/
*/
void test_create(void)
{
    queue_t q;

    q = queue_create();
    assert(q != NULL);
}

void test_queue_simple(void)
{
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

void test_empty_deq(void)
{
    queue_t q;
    int *ptr;

    q = queue_create();
    assert(queue_dequeue(q, (void**)&ptr) == -1);
    assert(queue_destroy(q) == 0);
}

void test_multi_elem(void)
{
    queue_t q;
    int data1 = 1, data2 = 2, data3 = 3;
    int *ptr;

    q = queue_create();
    
    assert(queue_enqueue(q, &data1) == 0);
    assert(queue_enqueue(q, &data2) == 0);
    assert(queue_enqueue(q, &data3) == 0);
    assert(queue_length(q) == 3);
    
    assert(queue_dequeue(q, (void**)&ptr) == 0);
    assert(ptr == &data1);
    assert(queue_dequeue(q, (void**)&ptr) == 0);
    assert(ptr == &data2);
    assert(queue_dequeue(q, (void**)&ptr) == 0);
    assert(ptr == &data3);
    
    assert(queue_length(q) == 0);
    assert(queue_destroy(q) == 0);
}

void test_queue_del(void)
{
    queue_t q;
    int data1 = 1, data2 = 2, data3 = 3, data4 = 4;
    int *ptr;

    q = queue_create();
    
    assert(queue_enqueue(q, &data1) == 0);
    assert(queue_enqueue(q, &data2) == 0);
    assert(queue_enqueue(q, &data3) == 0);
    assert(queue_enqueue(q, &data4) == 0);
    assert(queue_length(q) == 4);
    
    assert(queue_delete(q, &data2) == 0);
    assert(queue_length(q) == 3);
    
    assert(queue_dequeue(q, (void**)&ptr) == 0);
    assert(ptr == &data1);
    
    assert(queue_dequeue(q, (void**)&ptr) == 0);
    assert(ptr == &data3);
    
    assert(queue_dequeue(q, (void**)&ptr) == 0);
    assert(ptr == &data4);
    
    assert(queue_length(q) == 0);
    
    assert(queue_enqueue(q, &data1) == 0);
    assert(queue_delete(q, &data2) == -1);
    assert(queue_length(q) == 1);
    
    assert(queue_destroy(q) == -1);
}

void test_queue_resize(void)
{
    queue_t q;
    int data[10];
    int *ptr;
    int i;

    q = queue_create();
    
    for (i = 0; i < 10; i++){
        data[i] = i;
        assert(queue_enqueue(q, &data[i]) == 0);
    }
    
    assert(queue_length(q) == 10);
    
    for (i = 0; i < 10; i++){
        assert(queue_dequeue(q, (void**)&ptr) == 0);
        assert(ptr == &data[i]);
    }
    
    assert(queue_length(q) == 0);
    assert(queue_destroy(q) == 0);
}

void test_deq_order(void)
{
    queue_t q;
    int vals[5] = {1, 2, 3, 4, 5};
    int *ptr;
    int i;

    q = queue_create();
    
    for (i = 0; i < 5; i++){
        assert(queue_enqueue(q, &vals[i]) == 0);
    }
    for (i = 0; i < 5; i++){
        assert(queue_dequeue(q, (void**)&ptr) == 0);
        assert(ptr == &vals[i]);
    }
    
    for (i = 0; i < 5; i++){
        assert(queue_enqueue(q, &vals[i]) == 0);
    }    
    for (i = 0; i < 5; i++){
        assert(queue_dequeue(q, (void**)&ptr) == 0);
        assert(ptr == &vals[i]);
    }
    
    assert(queue_destroy(q) == 0);
}

int main(void)
{
    test_create();
    test_queue_simple();
    test_empty_deq();
    test_multi_elem();
    test_queue_del();
    test_queue_resize();
    test_deq_order();
    
    return 0;
}