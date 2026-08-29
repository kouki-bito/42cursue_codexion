NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror
DEBUG_FLAGS = -g3 -O0

PTHREAD     = -pthread

SRCS        = main.c \
              src/parse.c \
			  src/init.c \
			  src/dongle.c\
			  src/monitor.c\
			  src/thread_routing.c\
			  src/init_data.c \
			  src/destroy.c \
			  src/setter.c\
			  src/heap.c\
			  src/getter.c\
			  src/log.c\
			  src/check_time.c\
			  src/check_simulation.c\
			  src/create_thread.c\
			  src/sleep.c\
			  src/check_heap.c\
			  src/coder_action.c

			 

OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(PTHREAD) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(PTHREAD) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
debug: fclean
	$(MAKE) CFLAGS=" $(DEBUG_FLAGS)" all

.PHONY: all clean fclean re debug
