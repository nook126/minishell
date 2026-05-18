/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 12:00:00 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/09 12:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_dollar(char *val, int *i, t_shell *shell)
{
	char	*name;
	char	*value;
	int		start;

	(*i)++;
	if (val[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	start = *i;
	while (val[*i] && (ft_isalnum(val[*i]) || val[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup("$"));
	name = ft_substr(val, start, *i - start);
	if (!name)
		return (NULL);
	value = get_var(name, shell->vars);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static int	handle_squote(char *val, int i, char **res)
{
	char	buf[2];

	i++;
	buf[1] = '\0';
	while (val[i] && val[i] != '\'')
	{
		buf[0] = val[i];
		*res = ft_strjoin_free(*res, buf);
		i++;
	}
	if (val[i] == '\'')
		i++;
	return (i);
}

static int	handle_dquote(char *val, int i, char **res, t_shell *shell)
{
	char	*tmp;

	i++;
	while (val[i] && val[i] != '"')
	{
		if (val[i] == '$')
		{
			tmp = handle_dollar(val, &i, shell);
			*res = ft_strjoin_free(*res, tmp);
			free(tmp);
		}
		else
		{
			if (val[i] == '\\' && (val[i + 1] == '"' || val[i + 1] == '\\'))
				i++;
			tmp = ft_substr(val, i++, 1);
			*res = ft_strjoin_free(*res, tmp);
			free(tmp);
		}
	}
	i += (val[i] == '"');
	return (i);
}

char	*expand_token(char *val, t_shell *shell)
{
	char	*res;
	char	*tmp;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (val[i] && res)
	{
		if (val[i] == '\'')
			i = handle_squote(val, i, &res);
		else if (val[i] == '"')
			i = handle_dquote(val, i, &res, shell);
		else
		{
			if (val[i] == '$')
				tmp = handle_dollar(val, &i, shell);
			else
				tmp = ft_substr(val, i++, 1);
			res = ft_strjoin_free(res, tmp);
			free(tmp);
		}
	}
	return (res);
}

int	expand_token_list(t_token *head, t_shell *shell)
{
	char	*expanded;

	while (head)
	{
		expanded = expand_token(head->val, shell);
		if (!expanded)
			return (-1);
		free(head->val);
		head->val = expanded;
		head = head->next;
	}
	return (0);
}
