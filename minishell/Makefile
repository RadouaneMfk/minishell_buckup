NAME = minishell

SRC = env/shelvl.c main.c env/env_copy.c builtins/ft_env.c builtins/ft_pwd.c \
	  builtins/ft_cd.c builtins/ft_echo.c builtins/ft_unset.c builtins/ft_exit.c \
	  builtins/ft_export.c parsing/parse_cmd.c parsing/tokens.c execution/ft_execute.c \
	  execution/exec_multiple.c execution/signals.c parsing/check_syntaxe.c parsing/count.c \
	  parsing/expand.c

CC = cc
# CFLAGS = -Wall -Wextra -Werror
OBJ = $(SRC:.c=.o)
LIB = lib/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LIB) -lreadline -o $(NAME)
	@echo "\033[1;32m✔️  $(NAME) built successfully!\033[0m"
	@make clean

%.o: %.c minishell.h
	@$(CC) -g -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
