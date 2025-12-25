/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:43:33 by ariazano          #+#    #+#             */
/*   Updated: 2025/12/25 17:35:57 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

//signal variable
extern int	g_signal_received;

// COLORS
# define GREEN	"\033[0;32m"
# define RED	"\033[1;31m"
# define GREY	"\033[0;90m"
# define CYAN	"\033[1;96m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[1;34m"
# define RESET	"\033[0m"

// types of token |  &  ;  <  >  (  )  $  `  \  "  '  <space>  <tab>  <newline>
typedef enum e_token_type
{
	T_NEWLINE,
	T_SPACE,
	T_DOLLAR,
	T_AMPER,
	T_TAB,
	T_APPEND,
	T_PIPE,
	T_OR,
	T_AND,
	T_DELIM,
	T_LPAR,
	T_RPAR,
//	T_WORD = 1,
//	T_RED_INP,
//	T_RED_OUT,
//	T_THREE_IN,
//	T_THREE_OUT,
//	T_IN_OUT,
}			t_token_type;

typedef struct	s_shell
{
	char			**env;
	int				exit_status;
}				t_shell;

/*
typedef struct	s_data
{
	int		argc;
	char	**argv;
	char	*env;
}				t_data;

typedef struct	s_token
{
	t_token_type	type;
	char			*word;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;
*/

/* signals.c */
void	setup_signals(void);
void	handle_sigint(int sig);

/* env_utils.c */
int		env_size(char **env);
char	**copy_env(char **env);
void	free_env(char **env);
char	*get_env_var(char *name, char **env);

/* errors.c */
void	print_error(char *msg);
void 	exit_error(char *msg, int code);

/* find_path.c */
char	*find_command(char *cmd, char **env);
char	*get_path_env(char **env);
char	**split_path(char *path_env);
int		is_executable(char *path);
char	*join_path_cmd(char *dir, char *cmd);

/* execute.c */
int		execute_command(char **args, t_shell *shell);
int		fork_and_exec(char **args, t_shell *shell);
void	execute_child(char **args, char **env);
int		is_absolute_path(char *str);
int		is_relative_path(char *str);

/* string_utils.c */
void	free_array(char **arr);
int		array_len(char **arr);
char	*ft_strjoin_free(char *s1, char *s2);

/* tokenize.c */
char	**tokenize_input(char *input);
int		count_words(char *str);
char	*extract_word(char *str, int start, int len);
int		get_word_len(char *str, int start);
int		is_space(char c);














#endif