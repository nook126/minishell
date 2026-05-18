/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:43:33 by ariazano          #+#    #+#             */
/*   Updated: 2026/05/16 13:27:20 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <limits.h>
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

/* Token node — internal, used only during parsing */
typedef struct s_token
{
	char			*val;
	struct s_token	*next;
}	t_token;

/* Environment variable node */
typedef struct s_var
{
	char			*name;
	char			*value;
	int				exported;
	struct s_var	*next;
}	t_var;

/* Shell context — env linked list + last return code */
typedef struct s_shell
{
	int		in_child;
	t_var	*vars;
	int		exit_status;
}	t_shell;

/* Redirection type constants */
# define REDIR_IN		0
# define REDIR_OUT		1
# define REDIR_APPEND	2
# define REDIR_HEREDOC	3

/* Redirection node */
typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

/* Command node — parser output, one node per pipe segment */
typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

/* Pipes */
typedef struct s_pipe
{
	int		in;
	int		fd[2];
	int		cmd_count;
	pid_t	pid;
}	t_pipe;

/* Execution struct */
typedef struct s_exec
{
	t_shell			*shell;
	t_cmd			*cmd;
	t_pipe			pipe;
}	t_exec;

/* signals.c */
void	setup_signals(void);
void	handle_sigint(int sig);

/* var_list.c */
t_var	*new_var(char *name, char *value, int exported);
void	var_add_back(t_var **head, t_var *node);
t_var	*find_var(char *name, t_var *vars);
t_var	*envp_to_vars(char **envp);
int		var_count(t_var *vars);

/* env_utils.c */
char	*get_var(char *name, t_var *vars);
int		set_var(char *name, char *value, int exported, t_shell *shell);
void	unset_var(char *name, t_shell *shell);
void	free_vars(t_var *vars);
char	**vars_to_envp(t_var *vars);

/* errors.c */
void	print_error(char *msg);
void	exit_error(char *msg, int code);

/* find_path.c */
char	*find_command(char *cmd, t_var *vars);
char	*get_path_env(t_var *vars);
char	**split_path(char *path_env);
int		is_executable(char *path);
char	*join_path_cmd(char *dir, char *cmd);

/* builtin_commands */
int		exec_cd(t_cmd *cmd, t_shell *shell);
int		exec_echo(char **args);

int		exec_pwd(t_cmd *cmd, t_shell *shell);
int		exec_export(t_cmd *cmd, t_shell *shell);
int		exec_unset(t_cmd *cmd, t_shell *shell);
int		exec_env(t_shell *shell);
int		exec_exit(t_cmd *cmd, t_shell *shell);

/* builtin_entry.c */
int     save_std_fds(int *saved_stdin, int *saved_stdout);
void    restore_std_fds(int saved_stdin, int saved_stdout);
int		is_builtin(char *arg);

/* execute.c */
int		execute_command(t_cmd *cmd, t_shell *shell);
int		fork_and_exec(t_cmd *cmd, t_shell *shell);
void	execute_child(char **args, t_var *vars);
int		is_absolute_path(char *str);
int		is_relative_path(char *str);

/* redirection.c */
void	apply_redirections(t_redir *redir);

/* setup_pipes.c */
void	close_pipe_fds(int *fd);
void	setup_input_pipe(int fd);
void	setup_output_pipe(int *fd);
void	close_unused_pipes(int *prev_fd, int *curr_fd);

/* run_commands.c */
void	run_commands(t_cmd *cmds, t_shell *shell);
int		execute_builtin(t_cmd *cmd, t_shell *shell);
int		select_builtin(t_cmd *cmd, t_shell *shell);
int		execute_pipeline(t_cmd *cmds, t_shell *shell);

/* string_utils.c */
void	free_array(char **arr);
int		array_len(char **arr);
char	*ft_strjoin_free(char *s1, char *s2);

/* readline_utils.c */
char	*read_full_line(void);

/* tokenize.c */
t_token	*tokenize_input(char *input);

/* tokenize_utils.c */
int		is_space(char c);
int		is_quote(char c);
int		jump_past_quote(char *str, int i);
t_token	*new_token(char *val);
void	token_add_back(t_token **head, t_token *node);

/* token_free.c */
void	free_tokens(t_token *head);
char	**tokens_to_argv(t_token *head);
int		token_count(t_token *head);

/* split_pipes.c */
t_cmd	*split_and_parse(char *input, t_shell *shell);
int		find_pipe_pos(char *input, int start);
char	*extract_segment(char *input, int start, int end);

/* expand.c */
char	*expand_token(char *val, t_shell *shell);
int		expand_token_list(t_token *head, t_shell *shell);

/* cmd_utils.c */
t_cmd	*create_cmd(char **args);
void	free_cmd(t_cmd *cmd);
void	free_cmd_list(t_cmd *head);
int		cmd_count(t_cmd *head);
void	cmd_add_back(t_cmd **head, t_cmd *node);

/* redir_utils.c */
t_redir	*new_redir(int type, char *file);
void	redir_add_back(t_redir **head, t_redir *node);
void	free_redirs(t_redir *head);
int		is_redir_op(char *val);
int		get_redir_type(char *val);

/* redir_parse.c */
int		check_redir_syntax(char *input);
int		parse_redirs(t_token **head, t_redir **redirs);

/* syntax.c */
int		validate_input(char *input);
int		syntax_error(char *msg);
int		check_unclosed_quotes(char *input);


#endif
