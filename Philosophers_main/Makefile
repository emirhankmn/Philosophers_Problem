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

SRC_DIR = source
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/utils.c $(SRC_DIR)/actions.c $(SRC_DIR)/init.c

OBJECTS = $(SRC:.c=.o)

all: $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@echo $(Y)Compiling [$<]...
	@$(CC) $(CFLAGS) -c -o $@ $<
	@printf $(UP)$(CUT)

$(NAME): $(OBJECTS)
	@echo $(Y)Compiling [$(SRC)]...
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo $(G)Finished [$(NAME)]

clean:
	@rm -f $(OBJECTS)
	@echo $(R)Removed [$(OBJECTS)]

fclean: clean
	@rm -f $(NAME)
	@echo $(R)Removed [$(NAME)]

re: fclean all

.PHONY: all clean fclean re
