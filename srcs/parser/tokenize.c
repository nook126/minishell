/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 17:20:08 by aleriaza          #+#    #+#             */
/*   Updated: 2025/12/25 17:21:34 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	get_word_len(char *str, int start)
{
	int	len;

	len = 0;
	while (str[start + len] && !is_space(str[start + len]))
		len++;
	return (len);
}

char	*extract_word(char *str, int start, int len)
{
	char	*word;
	int		i;

	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[start + i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

int	count_words(char *str)
{
	int	count;
	int	i;
	int	in_word;

	count = 0;
	i = 0;
	in_word = 0;
	while (str[i])
	{
		if (!is_space(str[i]) && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (is_space(str[i]))
			in_word = 0;
		i++;
	}
	return (count);
}

char	**tokenize_input(char *input)
{
	char	**tokens;
	int		word_count;
	int		i;
	int		j;
	int		word_len;

	word_count = count_words(input);
	tokens = malloc(sizeof(char *) * (word_count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		word_len = get_word_len(input, i);
		tokens[j++] = extract_word(input, i, word_len);
		i += word_len;
	}
	tokens[j] = NULL;
	return (tokens);
}