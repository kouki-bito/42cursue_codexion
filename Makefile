NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

PTHREAD     = -pthread

SRCS        = main.c \
              src/parse.c \
			  src/init.c \
			  src/utilis.c \
			  src/dongle.c
			 

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