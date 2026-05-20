/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:43:43 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/16 13:58:24 by dprudnik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_commands(t_cmd *cmds, t_shell *shell)
{
	if (cmds->next == NULL)
		if (is_builtin(cmds->args[0]))
			execute_builtin(cmds, shell);
		else
			execute_command(cmds, shell);
	else if (cmds->next != NULL)
		execute_pipeline(cmds, shell);
	return ;
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;
	int saved_stdin;
	int saved_stdout;

	exit_code = 0;
	if (cmd->redirs == NULL)
		exit_code = select_builtin(cmd, shell);
	else
	{
		if (save_std_fds(&saved_stdin, &saved_stdout) == -1)
		{
			shell->exit_status = 1;
			return (-1);
		}
		apply_redirections(cmd->redirs);
		exit_code = select_builtin(cmd, shell);
		restore_std_fds(saved_stdin, saved_stdout);
	}
	shell->exit_status = exit_code;
	return (0);
}

int	select_builtin(t_cmd *cmd, t_shell *shell)
{
	int	len;
	char *arg;

	arg = cmd->args[0];
	len = ft_strlen(arg);
	if (ft_strncmp(arg, "echo", len) == 0)
		return (exec_echo(cmd->args));
	else if (ft_strncmp(arg, "cd", len) == 0)
		return (exec_cd(cmd, shell));
	else if (ft_strncmp(arg, "pwd", len) == 0)
		return (exec_pwd(cmd, shell));
	else if (ft_strncmp(arg, "export", len) == 0)
		return (exec_export(cmd, shell));
	else if (ft_strncmp(arg, "unset", len) == 0)
		return (exec_unset(cmd, shell));
	else if (ft_strncmp(arg, "env", len) == 0)
		return (exec_env(shell));
	else if (ft_strncmp(arg, "exit", len) == 0)
		return (exec_exit(cmd, shell));
	return (0);
}

int	open_pipe(t_exec *ex)
{
	if (!ex->cmd->next)
		return (0);
	if (pipe(ex->pipe.fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static void	wait_children(t_pipe *pipe, t_shell *shell)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == pipe->pid)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit (core dumped)\n");
			else if (WTERMSIG(status) == SIGINT)
				printf("\n");
		}
		pid = waitpid(-1, &status, 0);
	}
	setup_signals();
}

void	child_process(t_exec *ex)
{
	ex->shell->in_child = 1;
	if (ex->pipe.in != STDIN_FILENO)
		setup_input_pipe(ex->pipe.in);
	if (ex->cmd->next)
		setup_output_pipe(ex->pipe.fd);
	if (is_builtin(ex->cmd->args[0]))
		exit(execute_builtin(ex->cmd, ex->shell));
	else
	{
		if (ex->cmd->redirs != NULL)
			apply_redirections(ex->cmd->redirs);
		execute_child(ex->cmd->args, ex->shell->vars);
	}
}

void	parent_process(t_exec *ex)
{
	if (ex->pipe.in != STDIN_FILENO)
		close(ex->pipe.in);
	if (!ex->cmd->next)
		return ;
	close(ex->pipe.fd[1]);
	ex->pipe.in = ex->pipe.fd[0];
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	t_exec	ex;

	ex.shell = shell;
	ex.cmd = cmds;
	ex.pipe.in = STDIN_FILENO;
	ex.pipe.cmd_count = cmd_count(cmds);
	setup_tmp_signals();// NOTE : added here to test first.
	while (ex.cmd)
	{
		if (open_pipe(&ex))
			return (1);
		ex.pipe.pid = fork();
		if (ex.pipe.pid == -1)
			return (perror("fork"), 1);
		if (ex.pipe.pid == 0)
			child_process(&ex);
		parent_process(&ex);
		ex.cmd = ex.cmd->next;
	}
	wait_children(&ex.pipe, shell);
	return (0);
}
