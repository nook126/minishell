/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:32:39 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/18 00:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Print all exported vars in declare -x format (export with no args) */
static void	exec_export_no_args(t_var *vars)
{
	while (vars)
	{
		if (vars->exported)
			ft_printf("declare -x %s=\"%s\"\n", vars->name, vars->value);
		vars = vars->next;
	}
}

/* Set or create vars as exported; no args prints current exported vars */
int	exec_export(t_cmd *cmd, t_shell *shell)
{
	t_var	*node;
	char	*eq;
	int		i;

	if (!cmd->args[1])
		return (exec_export_no_args(shell->vars), 0);
	i = 1;
	while (cmd->args[i])
	{
		eq = ft_strchr(cmd->args[i], '=');
		if (eq)
		{
			*eq = '\0';
			set_var(cmd->args[i], eq + 1, 1, shell);
			*eq = '=';
		}
		else
		{
			node = find_var(cmd->args[i], shell->vars);
			if (node)
				node->exported = 1;
			else
				set_var(cmd->args[i], NULL, 1, shell);
		}
		i++;
	}
	return (0);
}

/* Remove variables from the shell var list */
int	exec_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		unset_var(cmd->args[i], shell);
		i++;
	}
	return (0);
}

/* Print all exported environment variables as NAME=VALUE */
int	exec_env(t_shell *shell)
{
	t_var	*cur;

	cur = shell->vars;
	while (cur)
	{
		if (cur->exported)
			ft_printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
	return (0);
}

/* Exit the shell with optional exit code */
int	exec_exit(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->args[1])
	{
		print_error("exit: too many arguments");
		exit_code = 2;
	}
	if (shell->in_child == 0)
		write(1, "exit\n", 5);
	free_cmd_list(cmd);
	if (shell->vars)
		free_vars(shell->vars);
	exit(exit_code);
}
