NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

PTHREAD     = -pthread

SRCS        = main.c \
              src/parse.c \
			  src/init.c \
			  src/dongle.c\
			  src/list.c\
			  src/monitor.c\
			  src/mutex_utilis.c\
			  src/sort.c\
			  src/thread_routing.c\
			  src/time_utilis.c \
			  src/destroy.c

			 

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

.PHONY: all clean fclean re