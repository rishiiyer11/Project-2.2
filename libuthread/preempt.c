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
*/

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
#define USEC_PER_SEC 1000000

/*
static struct sigaction old_action;
static struct itimerval  old_timer;
static sigset_t timer_mask;

static void handle_timer(int sig)
{
	if (sig == SIGVTALRM) {
		uthread_yield();
	}
}

void preempt_disable(void)
{
	sigprocmask(SIG_BLOCK, &timer_mask, NULL);
}

void preempt_enable(void)
{
	sigprocmask(SIG_UNBLOCK, &timer_mask, NULL);
}

void preempt_start(bool preempt)
{
	if (!preempt) {
		return;
	}
	sigemptyset(&timer_mask);
    sigaddset(&timer_mask, SIGVTALRM);

    // install our handler, saving the old one
    struct sigaction sa;
    sa.sa_handler = handle_timer;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    assert(sigaction(SIGVTALRM, &sa, &old_action) == 0);

    // arm the virtual timer: first and recurring intervals
    struct itimerval tv;
    tv.it_interval.tv_sec  = 0;
    tv.it_interval.tv_usec = USEC_PER_SEC / HZ;
    tv.it_value = tv.it_interval;
    assert(setitimer(ITIMER_VIRTUAL, &tv, &old_timer) == 0);
}

void preempt_stop(void)
{
	sigaction(SIGVTALRM, &old_action, NULL);
    setitimer(ITIMER_VIRTUAL, &old_timer, NULL);
}
*/

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



