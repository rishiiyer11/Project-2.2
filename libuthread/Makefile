CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
CFLAGS += -g
AR      = ar
ARFLAGS = rcs

# Target library
LIB     = libuthread.a
OBJDIR  = .
OBJS    = $(OBJDIR)/queue.o

all: $(LIB)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all clean