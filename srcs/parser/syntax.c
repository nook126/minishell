/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 12:00:00 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/09 12:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	syntax_error(char *msg)
{
	ft_putstr_fd("minishell: syntax error: ", 2);
	ft_putendl_fd(msg, 2);
	return (-1);
}

int	check_unclosed_quotes(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (is_quote(input[i]))
		{
			i = jump_past_quote(input, i);
			if (i == -1)
				return (-1);
		}
		else
			i++;
	}
	return (0);
}

static int	check_pipe_char(char *input, int *i, int *has_content, int *had_pipe)
{
	if (is_quote(input[*i]))
	{
		*i = jump_past_quote(input, *i);
		if (*i == -1)
			return (-1);
		*has_content = 1;
	}
	else if (input[*i] == '|')
	{
		if (!*has_content)
			return (-1);
		*has_content = 0;
		*had_pipe = 1;
		(*i)++;
	}
	else if (!is_space(input[*i]))
	{
		*has_content = 1;
		(*i)++;
	}
	else
		(*i)++;
	return (0);
}

static int	check_pipe_syntax(char *input)
{
	int	i;
	int	has_content;
	int	had_pipe;

	i = 0;
	has_content = 0;
	had_pipe = 0;
	while (input[i])
	{
		if (check_pipe_char(input, &i, &has_content, &had_pipe) == -1)
			return (-1);
	}
	if (had_pipe && !has_content)
		return (-1);
	return (0);
}

int	validate_input(char *input)
{
	if (check_unclosed_quotes(input) == -1)
		return (syntax_error("unclosed quote"));
	if (check_pipe_syntax(input) == -1)
		return (syntax_error("syntax error near unexpected token `|'"));
	if (check_redir_syntax(input) == -1)
		return (syntax_error("syntax error near unexpected token `newline'"));
	return (0);
}
