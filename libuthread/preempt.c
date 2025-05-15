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
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
#define USEC_PER_SEC 1000000

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
	/* TODO Phase 4 */
	sigprocmask(SIG_BLOCK, &timer_mask, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	sigprocmask(SIG_UNBLOCK, &timer_mask, NULL);
}

void preempt_start(bool preempt)
{
	/* TODO Phase 4 */
	if (!preempt) {
		return;
	}
	sigemptyset(&timer_mask);
    sigaddset(&timer_mask, SIGVTALRM);

    /* install our handler, saving the old one */
    struct sigaction sa;
    sa.sa_handler = handle_timer;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    assert(sigaction(SIGVTALRM, &sa, &old_action) == 0);

    /* arm the virtual timer: first and recurring intervals */
    struct itimerval tv;
    tv.it_interval.tv_sec  = 0;
    tv.it_interval.tv_usec = USEC_PER_SEC / HZ;
    tv.it_value = tv.it_interval;
    assert(setitimer(ITIMER_VIRTUAL, &tv, &old_timer) == 0);
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
	sigaction(SIGVTALRM, &old_action, NULL);
    setitimer(ITIMER_VIRTUAL, &old_timer, NULL);
}

