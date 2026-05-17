/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:55:47 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/09 12:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	jump_past_quote(char *str, int i)
{
	char	qchar;

	qchar = str[i];
	i++;
	while (str[i] && str[i] != qchar)
	{
		if (qchar == '"' && str[i] == '\\'
			&& (str[i + 1] == qchar || str[i + 1] == '\\'))
			i++;
		i++;
	}
	if (!str[i])
		return (-1);
	return (i + 1);
}

t_token	*new_token(char *val)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->val = val;
	node->next = NULL;
	return (node);
}

void	token_add_back(t_token **head, t_token *node)
{
	t_token	*cur;

	if (!head || !node)
		return ;
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
