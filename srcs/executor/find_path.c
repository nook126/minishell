/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 17:29:15 by aleriaza          #+#    #+#             */
/*   Updated: 2025/12/25 17:33:03 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Get PATH value from the var linked list */
char	*get_path_env(t_var *vars)
{
	return (get_var("PATH", vars));
}

/* Join directory path with command name */
char	*join_path_cmd(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

int	is_executable(char *path)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (1);
	}
	return (0);
}

/* Split by ':' */
char	**split_path(char *path_env)
{
	char	**paths;

	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}

char	*find_command(char *cmd, t_var *vars)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = split_path(get_path_env(vars));
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = join_path_cmd(paths[i], cmd);
		if (is_executable(full_path))
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}
