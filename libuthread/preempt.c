#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

#include "private.h"
#include "uthread.h"

/*
Resources:
https://stackoverflow.com/questions/3417837/best-way-to-suppress-c-warning-unused-variable-x
https://www.geeksforgeeks.org/preemptive-and-non-preemptive-scheduling/
https://man7.org/linux/man-pages/man2/sigaction.2.html
https://man7.org/linux/man-pages/man2/setitimer.2.html
https://hpc-tutorials.llnl.gov/posix/
https://pages.cs.wisc.edu/~remzi/Classes/537/Fall2008/Projects/p2a.html
*/

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
*/

#define HZ 100
#define USEC_PER_SEC 1000000

static struct sigaction ogAct;
static struct itimerval  ogTime;
static sigset_t preemptSig;

// yield curr thread
static void preemptHandler(int signo){

	// suppresses unused flag
	(void) signo;
	uthread_yield();
}

void preempt_disable(void){
	sigprocmask(SIG_BLOCK, &preemptSig, NULL);
}

void preempt_enable(void){
	sigprocmask(SIG_BLOCK, &preemptSig, NULL);
}

void preempt_start(bool pre)
{
	if (!pre) {
		return;
	}
	sigemptyset(&preemptSig);
    sigaddset(&preemptSig, SIGVTALRM);

    // setup for sigHand
    struct sigaction sa;
    sa.sa_handler = preemptHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

	if (sigaction(SIGVTALRM, &sa, &ogAct) == -1) {
        perror("sigaction");
        exit(1);
    }

    // virt timer
    struct itimerval tim;

	// interval
    tim.it_interval.tv_sec  = 0;
    tim.it_interval.tv_usec = USEC_PER_SEC / HZ;

	// expir.
    tim.it_value = tim.it_interval;

	// start and save
	if(setitimer(ITIMER_VIRTUAL, &tim, &ogTime)){
		perror("setitimer");
		exit(1);
	}

}

void preempt_stop(void){
	// restore sig
	if(sigaction(SIGVTALRM, &ogAct, NULL) == -1){
		perror("sig restore");
		exit(1);
	}

	// restore time
	if(setitimer(ITIMER_VIRTUAL, &ogTime, NULL) == -1){
		perror("time restore");
		exit(1);
	}
}



