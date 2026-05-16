/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dprudnik <dprudnik@student.42wolfsburg.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:32:39 by dprudnik          #+#    #+#             */
/*   Updated: 2026/05/16 13:59:20 by dprudnik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// TODO : may need to change to use getcwd().
int	exec_pwd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (cmd->args[1])
	{
		print_error("pwd: too many arguments");
		shell->exit_status = 2;
		return (2);
	}
    path = getcwd(NULL, 0);
    if (!path) {
        print_error("pwd");
        return (1);
    }
	ft_printf("%s\n", path);
    free(path);
	return (0);
}

int	exec_export(t_cmd *cmd, t_shell *shell)
{
	(void)cmd;//tmp
	(void)shell;//tmp

	return (0);
}

int	exec_unset(t_cmd *cmd, t_shell *shell)
{
	(void)cmd;//tmp
	(void)shell;//tmp

	return (0);
}

int	exec_env(t_shell *shell)
{
	int	i = 0;

	while (shell->env[i])
	{
		ft_printf("%s\n", shell->env[i]);
		i++;
	}
	return (0);
}

int	exec_exit(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->args[1])
	{
		print_error("exit: too many arguments");
		exit_code = 2;
	}
	// printf("%d", shell->in_child);
	if (shell->in_child == 0)
		write(1, "exit\n", 5);
	free_cmd_list(cmd);
	if (shell->env)
		free_env(shell->env);
	exit(exit_code);
}
