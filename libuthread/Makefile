# libuthread/Makefile

# tools
CC      := gcc
AR      := ar
CFLAGS  := -Wall -Wextra -Werror -I. -MMD -MP
ARFLAGS := rcs

# library
LIB     := libuthread.a
SRCS    := queue.c uthread.c sem.c preempt.c
OBJS    := $(SRCS:.c=.o)
DEPS    := $(SRCS:.c=.d)

# executables (autograder targets)
EXES    := \
	queue_tester.x \
	uthread_hello.x \
	uthread_yield.x \
	sem_simple.x \
	sem_count.x \
	sem_buffer.x \
	sem_prime.x \
	test_preempt.x

.PHONY: all clean

all: $(LIB) $(EXES)

# build the static library
$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# compile C sources to objects, track dependencies
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

queue_tester.x: ../apps/queue_tester_example.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

uthread_hello.x: ../apps/uthread_hello.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

uthread_yield.x: ../apps/uthread_yield.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

sem_simple.x: ../apps/sem_simple.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

sem_count.x: ../apps/sem_count.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

sem_buffer.x: ../apps/sem_buffer.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

sem_prime.x: ../apps/sem_prime.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

test_preempt.x: ../apps/test_preempt.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -luthread -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(LIB) $(EXES)
