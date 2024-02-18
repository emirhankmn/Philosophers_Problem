# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eakman <eakman@student.42kocaeli.com.tr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/18 16:54:14 by eakman            #+#    #+#              #
#    Updated: 2024/02/18 19:32:04 by eakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc

CFLAGS = -Wall -Werror -Wextra -g

Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

SRC = main.c init.c utils.c threads.c actions.c \

OBJECTS = $(SRC:.c=.o)

all: libraries $(NAME)

%.o : %.c
	@echo $(Y)Compiling [$<]...
	@$(CC) $(CFLAGS) -c -o $@ $<
	@printf $(UP)$(CUT)

libraries:

$(NAME): $(OBJECTS)
	@echo $(Y)Compiling [$(SRC)]...
	@echo $(G)Finished [$(SRC)]
	@echo $(Y)Compiling [$(NAME)]...
	@$(CC) $(CFLAGS) $(LIBFT_LIB) $(PRINTF_LIB) $(OBJECTS) -o $(NAME)
	@echo $(G)Finished [$(NAME)]

clean:
	@rm -f $(OBJECTS)
	@echo $(R)Removed [$(OBJECTS)]
	@echo $(R)Removed libraries.o

fclean: clean
	@rm -f $(NAME)
	@echo $(R)Removed [$(NAME)]

re: fclean all

.PHONY: all clean fclean re norm