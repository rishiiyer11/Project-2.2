#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "uthread.h"
#include "private.h"

/*
Resources:
https://www.geeksforgeeks.org/preemptive-and-non-preemptive-scheduling/
https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Blocking-Signals
https://stackoverflow.com/questions/3417837/best-way-to-suppress-c-warning-unused-variable-x
*/

// num iterations
#define NUM_ITERS 1000000

// store thread exec
volatile int seq[100];
volatile int seqIndex = 0;
volatile int completed = 0;

// get TID
void threadID(int TID) {
    if (seqIndex < 100) {
        seq[seqIndex++] = TID;
    }
}

// busy func
void busy(int iterations) {
    volatile int counter = 0;
    for (int i = 0; i < iterations; i++) {
        counter++;
    }
}

// thread func
void tFunc(void *arg) {
    int TID = *(int*)arg;
    
    printf("Thread %d starting\n", TID);
    
    // store TID
    threadID(TID);
    
    // loop
    for (int i = 0; i < 5; i++) {
        // print
        printf("Thread %d: iter %d\n", TID, i);
        
        // busy idle
        busy(NUM_ITERS / 10);
        
        // store thread again
        threadID(TID);
    }
    
    printf("Thread %d done\n", TID);
    completed++;
}

int main(void) {
    int TID1 = 1;
    int TID2 = 2;
    int thread1, thread2;

    // start pre
    preempt_start(true);
    
    // create threads
    thread1 = uthread_create(tFunc, &TID1);
    if (thread1 < 0) {
        fprintf(stderr, "fail to make t1\n");
        exit(1);
    }
    
    thread2 = uthread_create(tFunc, &TID2);
    if (thread2 < 0) {
        fprintf(stderr, "fail to make t2\n");
        exit(1);
    }
    
    // wait
    while (completed < 2) {
    }
    
    // stop pre
    preempt_stop();
    
    // print
    printf("first %d thread switch:\n", seqIndex);
    for (int i = 0; i < seqIndex; i++) {
        printf("%d ", seq[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    
    // check preempt
    bool detPreempt = false;
    for (int i = 1; i < seqIndex; i++) {
        if (seq[i] != seq[i-1]) {
            detPreempt = true;
            break;
        }
    }
    
    if (detPreempt) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    
    return 0;
}