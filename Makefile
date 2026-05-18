NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

INCLUDES = -Iincludes/ -Ilibft/

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJ_DIR = obj

SRCS =	srcs/main.c \
		srcs/signals/signals.c \
		srcs/utils/env_utils.c \
		srcs/utils/var_list.c \
		srcs/utils/errors.c \
		srcs/utils/string_utils.c \
		srcs/utils/readline_utils.c \
		srcs/parser/tokenize.c \
		srcs/parser/tokenize_utils.c \
		srcs/parser/token_free.c \
		srcs/parser/split_pipes.c \
		srcs/parser/cmd_utils.c \
		srcs/parser/expand.c \
		srcs/parser/redir_utils.c \
		srcs/parser/redir_parse.c \
		srcs/parser/syntax.c \
		srcs/executor/builtin_utils.c \
		srcs/executor/execute.c \
		srcs/executor/run_commands.c \
		srcs/executor/find_path.c \
		srcs/executor/setup_pipes.c \
		srcs/builtin_commands/builtin_cmds.c \
		srcs/builtin_commands/cd.c \
		srcs/builtin_commands/echo.c \
		srcs/executor/redirection.c

OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
