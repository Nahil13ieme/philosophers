NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g

SRCS = main.c utils.c init.c philo_routine.c monitor_routine.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all