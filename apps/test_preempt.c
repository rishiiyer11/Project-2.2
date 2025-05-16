#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <uthread.h>

/*
Resources:
https://www.geeksforgeeks.org/preemptive-and-non-preemptive-scheduling/
https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Blocking-Signals
https://stackoverflow.com/questions/3417837/best-way-to-suppress-c-warning-unused-variable-x
*/

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
        if (i % 10 == 0) {
            printf("print thread iter %d busy = %d\n", i, busyCnt);
            printCnt++;
        }
    }
    
    printf("thread done with %d prints\n", printCnt);
}

int main(void)
{
    printf("start preempt test for %d secs\n", 3);
    
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