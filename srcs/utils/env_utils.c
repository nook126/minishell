/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 16:30:39 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/18 00:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Return the value string for a variable name, or NULL if not found */
char	*get_var(char *name, t_var *vars)
{
	t_var	*node;

	node = find_var(name, vars);
	if (node)
		return (node->value);
	return (NULL);
}

/* Update an existing var or create a new one; NULL value keeps old value */
int	set_var(char *name, char *value, int exported, t_shell *shell)
{
	t_var	*node;

	node = find_var(name, shell->vars);
	if (node)
	{
		if (value != NULL)
		{
			free(node->value);
			node->value = ft_strdup(value);
		}
		if (exported)
			node->exported = 1;
		return (0);
	}
	node = new_var(name, value, exported);
	if (!node)
		return (-1);
	var_add_back(&shell->vars, node);
	return (0);
}

/* Remove a variable by name from the shell var list */
void	unset_var(char *name, t_shell *shell)
{
	t_var	*cur;
	t_var	*prev;

	cur = shell->vars;
	prev = NULL;
	while (cur)
	{
		if (find_var(name, cur) == cur)
		{
			if (prev)
				prev->next = cur->next;
			else
				shell->vars = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

/* Free the entire var linked list */
void	free_vars(t_var *vars)
{
	t_var	*next;

	while (vars)
	{
		next = vars->next;
		free(vars->name);
		free(vars->value);
		free(vars);
		vars = next;
	}
}

/* Build a char** of NAME=VALUE for exported vars (caller must free_array) */
char	**vars_to_envp(t_var *vars)
{
	char	**envp;
	t_var	*cur;
	int		i;

	envp = malloc(sizeof(char *) * (var_count(vars) + 1));
	if (!envp)
		return (NULL);
	cur = vars;
	i = 0;
	while (cur)
	{
		if (cur->exported)
		{
			envp[i] = ft_strjoin(cur->name, "=");
			envp[i] = ft_strjoin_free(envp[i], cur->value);
			i++;
		}
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}
