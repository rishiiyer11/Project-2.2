scheduler: scheduler.c scheduler.h
	gcc -Wall -Wextra -Werror -o scheduler scheduler.c

clean:
	rm -f scheduler