/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 16:06:23 by aleriaza          #+#    #+#             */
/*   Updated: 2025/12/25 17:37:12 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_env(shell->env);
	free(shell);
}

static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		exit_error("malloc failed", 1);
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		free(shell);
		exit_error("env copy failed", 1);
	}
	shell->exit_status = 0;
	return (shell);
}

static void	process_line(char *line, t_shell *shell)
{
	char **tokens;
	
	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	tokens = tokenize_input(line);
	if (!tokens)
		return ;
	execute_command(tokens, shell);
	free_array(tokens);
	printf("You typed: %s\n", line);
	(void)shell;
}

static void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
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
