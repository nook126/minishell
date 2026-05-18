/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 16:06:23 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/18 16:23:54 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Free shell and exit */
static void	cleanup_shell(t_shell *shell)
{
	if (shell)
	{
		if (shell->vars)
			free_vars(shell->vars);
		free(shell);
	}
}

static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		exit_error("malloc failed", 1);
	shell->vars = envp_to_vars(envp);
	if (!shell->vars)
	{
		free(shell);
		exit_error("env copy failed", 1);
	}
	shell->in_child = 0;
	shell->exit_status = 0;
	return (shell);
}

static void	process_line(char *line, t_shell *shell)
{
	t_cmd	*cmds;

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	if (validate_input(line) == -1)
	{
		shell->exit_status = 2;
		return ;
	}
	cmds = split_and_parse(line, shell);
	if (!cmds)
		return ;
	run_commands(cmds, shell);// NOTE :Added by Denis for exec entry point!!
	free_cmd_list(cmds);
}

static void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = read_full_line();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break ;
		}
		process_line(line, shell);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	setup_signals();
	shell_loop(shell);
	cleanup_shell(shell);
	return (0);
}
