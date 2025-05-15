#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <uthread.h>

#define TEST_DURATION 3
#define PRINT_INTERVAL 10

static volatile int busyCnt = 0;
static volatile int printCnt = 0;

static void busy_thread(void *arg)
{
    (void)arg;
    
    printf("busy thread start\n");
    
    while (1) {
        for (volatile int i = 0; i < 100000; i++) {
        }
        
        busyCnt++;
    }
}

static void print_thread(void *arg)
{
    (void)arg;
    
    printf("print thread start\n");
    
    for (int i = 0; i < 1000; i++) {
        if (i % PRINT_INTERVAL == 0) {
            printf("print thread iter %d busy = %d\n", i, busyCnt);
            printCnt++;
        }
    }
    
    printf("thread done with %d prints\n", printCnt);
}

int main(void)
{
    printf("start preempt test for %d secs\n", TEST_DURATION);
    
    bool preempt = true;
    
    uthread_run(preempt, print_thread, NULL);
    
    printf("done.\n");
    printf("busy loop count: %d\n", busyCnt);
    printf("print thread msgs: %d\n", printCnt);
    
    if (busyCnt > 0 && printCnt > 0) {
        printf("PASSED: threads progressed\n");
        return 0;
    } else {
        printf("FAILED: thread stuck\n");
        return 1;
    }
}