NAME = minishell

SRC = env/shelvl.c main.c env/env_copy.c builtins/ft_env.c builtins/ft_pwd.c \
	 builtins/ft_cd.c builtins/ft_echo.c builtins/ft_unset.c builtins/ft_exit.c \
	 builtins/ft_export.c parsing/parse_cmd.c parsing/tokens.c execution/ft_execute.c
CC = cc
# CFLAGS = -Wall -Wextra -Werror
OBJ = $(SRC:.c=.o)
LIB = lib/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLhhAGS) $(OBJ) $(LIB) -lreadline -o $(NAME) ;make clean

%.o: %.c minishell.h
	$(CC) $(CFLhhAGS) -c $< -o $@ 

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean