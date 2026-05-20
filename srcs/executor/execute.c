/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 17:23:08 by aleriaza          #+#    #+#             */
/*   Updated: 2025/12/25 17:28:10 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check if command starts with / */
int	is_absolute_path(char *str)
{
	if (str[0] == '/')
		return (1);
	return (0);
}

/* Check if command starts with ./ or ../ */
int	is_relative_path(char *str)
{
	if (str[0] == '.')
	{
		if (str[1] == '/')
			return (1);
		if (str[1] == '.' && str[2] == '/')
			return (1);
	}
	return (0);
}

/* Build envp from vars, exec the command; child exits on any failure */
void	execute_child(char **args, t_var *vars)
{
	char	**envp;
	char	*cmd_path;

	envp = vars_to_envp(vars);
	if (is_absolute_path(args[0]) || is_relative_path(args[0]))
		cmd_path = args[0];
	else
		cmd_path = find_command(args[0], vars);
	if (!cmd_path)
	{
		print_error("command not found");
		exit(127);
	}
	signal(SIGINT, SIG_DFL);// NOTE : Reset the signal to mormal for parent prosses.
	signal(SIGQUIT, SIG_DFL);// NOTE : Reset the signal to mormal for parent prosses.
	execve(cmd_path, args, envp);
	perror("execve");
	exit(126);
}

int	fork_and_exec(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	setup_tmp_signals();// NOTE : tmp signals for child prosses.
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (cmd->redirs != NULL)
			apply_redirections(cmd->redirs);
		execute_child(cmd->args, shell->vars);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))//added with tmp signals.
	{
		if (WTERMSIG(status) == SIGQUIT)
			printf("Quit (core dumped)\n");
		else if (WTERMSIG(status) == SIGINT)
			printf("\n");
	}
	setup_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	if (!cmd->args || !cmd->args[0])
		return (0);
	exit_code = fork_and_exec(cmd, shell);
	shell->exit_status = exit_code;
	return (exit_code);
}
