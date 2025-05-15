CC       := gcc
AR       := ar
CFLAGS   := -Wall -Wextra -Werror -I. -MMD -MP
ARFLAGS  := rcs

LIB      := libuthread.a
SRCS     := queue.c uthread.c sem.c
OBJS     := $(SRCS:.c=.o)
DEPS     := $(OBJS:.o=.d)

APPS     := queue_tester.x \
            uthread_hello.x \
            uthread_yield.x \
            sem_simple.x \
            sem_count.x \
            sem_buffer.x \
            sem_prime.x

.PHONY: all clean

all: $(LIB) $(APPS)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

%.x: ../apps/%.c $(LIB)
	$(CC) $(CFLAGS) ../apps/$*.c -L. -luthread -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(LIB) $(APPS)