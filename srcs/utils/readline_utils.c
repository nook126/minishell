/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 00:00:00 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/13 00:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*append_line(char *base, char *cont)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin_free(base, "\n");
	if (!tmp)
	{
		free(cont);
		return (NULL);
	}
	result = ft_strjoin(tmp, cont);
	free(tmp);
	free(cont);
	return (result);
}

static char	*read_noninteractive(void)
{
	char	buf[4096];
	size_t	len;

	if (!fgets(buf, sizeof(buf), stdin))
		return (NULL);
	len = ft_strlen(buf);
	if (len > 0 && buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	return (ft_strdup(buf));
}

char	*read_full_line(void)
{
	char	*line;
	char	*next;

	if (!isatty(STDIN_FILENO))
		return (read_noninteractive());
	line = readline("minishell> ");
	if (!line)
		return (NULL);
	while (check_unclosed_quotes(line) == -1)
	{
		next = readline("> ");
		if (!next)
		{
			free(line);
			return (NULL);
		}
		line = append_line(line, next);
		if (!line)
			return (NULL);
	}
	return (line);
}
