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

#include "../../includes/minishell.h"

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

void	execute_child(char **args, char **env)
{
	char	*cmd_path;

	if (is_absolute_path(args[0]) || is_relative_path(args[0]))
		cmd_path = args[0];
	else
		cmd_path = find_command(args[0], env);
	if (!cmd_path)
	{
		print_error("command not found");
		exit(127);
	}
	execve(cmd_path, args, env);
	perror("execve");
	exit(126);
}

int	fork_and_exec(char **args, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		execute_child(args, shell->env);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_command(char **args, t_shell *shell)
{
	int	exit_code;

	if (!args || !args[0])
		return (0);
	exit_code = fork_and_exec(args, shell);
	shell->exit_status = exit_code;
	return (exit_code);
}
