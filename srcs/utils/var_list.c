/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 00:00:00 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/18 00:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Allocate a new env variable node, duplicating name and value */
t_var	*new_var(char *name, char *value, int exported)
{
	t_var	*node;

	node = malloc(sizeof(t_var));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	node->value = ft_strdup(value ? value : "");
	node->exported = exported;
	node->next = NULL;
	if (!node->name || !node->value)
	{
		free(node->name);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

/* Append a node to the end of a var list */
void	var_add_back(t_var **head, t_var *node)
{
	t_var	*cur;

	if (!*head)
	{
		*head = node;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

/* Find a var node by exact name match */
t_var	*find_var(char *name, t_var *vars)
{
	int	len;

	len = ft_strlen(name);
	while (vars)
	{
		if (ft_strncmp(vars->name, name, len) == 0
			&& vars->name[len] == '\0')
			return (vars);
		vars = vars->next;
	}
	return (NULL);
}

/* Parse char **envp (NAME=VALUE strings) into a t_var linked list */
t_var	*envp_to_vars(char **envp)
{
	t_var	*head;
	t_var	*node;
	char	*eq;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			*eq = '\0';
			node = new_var(envp[i], eq + 1, 1);
			*eq = '=';
			if (!node)
				return (free_vars(head), NULL);
			var_add_back(&head, node);
		}
		i++;
	}
	return (head);
}

/* Count exported variables (used to size the envp array for execve) */
int	var_count(t_var *vars)
{
	int	count;

	count = 0;
	while (vars)
	{
		if (vars->exported)
			count++;
		vars = vars->next;
	}
	return (count);
}
