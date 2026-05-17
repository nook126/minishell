/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 15:09:35 by aleriaza          #+#    #+#             */
/*   Updated: 2026/05/09 12:00:00 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	find_pipe_pos(char *input, int start)
{
	int	i;
	int	ret;

	i = start;
	while (input[i])
	{
		if (is_quote(input[i]))
		{
			ret = jump_past_quote(input, i);
			if (ret == -1)
				return (-1);
			i = ret;
		}
		else if (input[i] == '|')
			return (i);
		else
			i++;
	}
	return (i);
}

char	*extract_segment(char *input, int start, int end)
{
	char	*seg;
	int		len;
	int		i;

	len = end - start;
	seg = malloc(sizeof(char) * (len + 1));
	if (!seg)
		return (NULL);
	i = 0;
	while (i < len)
	{
		seg[i] = input[start + i];
		i++;
	}
	seg[i] = '\0';
	return (seg);
}

static t_cmd	*segment_fail(t_token *tok, t_redir *red, char **args)
{
	free_tokens(tok);
	free_redirs(red);
	free_array(args);
	return (NULL);
}

static t_cmd	*parse_segment(char *seg, t_shell *shell)
{
	t_token	*tokens;
	t_redir	*redirs;
	char	**args;
	t_cmd	*cmd;

	tokens = tokenize_input(seg);
	if (!tokens)
		return (NULL);
	if (expand_token_list(tokens, shell) == -1)
		return (segment_fail(tokens, NULL, NULL));
	redirs = NULL;
	if (parse_redirs(&tokens, &redirs) == -1)
		return (segment_fail(tokens, redirs, NULL));
	args = tokens_to_argv(tokens);
	free_tokens(tokens);
	if (!args)
		return (segment_fail(NULL, redirs, NULL));
	cmd = create_cmd(args);
	if (!cmd)
		return (segment_fail(NULL, redirs, args));
	cmd->redirs = redirs;
	return (cmd);
}

t_cmd	*split_and_parse(char *input, t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*node;
	char	*seg;
	int		start;
	int		pipe;

	head = NULL;
	start = 0;
	while (1)
	{
		pipe = find_pipe_pos(input, start);
		if (pipe < 0 || !(seg = extract_segment(input, start, pipe)))
			return (free_cmd_list(head), NULL);
		node = parse_segment(seg, shell);
		free(seg);
		if (node)
			cmd_add_back(&head, node);
		if (!input[pipe])
			break ;
		start = pipe + 1;
	}
	return (head);
}
