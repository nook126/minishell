/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleriaza <aleriaza@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:29:00 by denis             #+#    #+#             */
/*   Updated: 2026/05/16 12:11:55 by aleriaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// TODO : Do I need to handle all chdir() errors???
// TODO : Need to updated OLDPWD in env after every cd
// and need to handle if get_env_var("OLDPWD", retuns NULL.
// BUG : Need to fix! Info in Things to discuss file.
static int	cd_error(char *path)
{
	char	*msg;

	msg = ft_strjoin("minishell: cd: ", path);
	perror(msg);
	free(msg);
	return (1);
}

/* Resolves target path: HOME, OLDPWD (with print), or literal arg */
static char	*get_cd_path(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->args[1])
		return (get_var("HOME", shell->vars));
	if (cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
	{
		path = get_var("OLDPWD", shell->vars);
		if (!path)
			return (print_error("cd: OLDPWD not set"), NULL);
		ft_printf("%s\n", path);
		return (path);
	}
	return (cmd->args[1]);
}

/* Changes directory and updates PWD and OLDPWD in env */
int	exec_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	buf[PATH_MAX];

	if (cmd->args[1] && cmd->args[2])
		return (print_error("cd: too many arguments"), 1);
	old_pwd = get_var("PWD", shell->vars);
	path = get_cd_path(cmd, shell);
	if (!path || path[0] == '\0')
		return (1);
	if (chdir(path) == -1)
		return (cd_error(path));
	set_var("OLDPWD", old_pwd ? old_pwd : "", 1, shell);
	set_var("PWD", getcwd(buf, PATH_MAX), 1, shell);
	return (0);
}

/* Print current working directory */
int	exec_pwd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (cmd->args[1])
	{
		print_error("pwd: too many arguments");
		shell->exit_status = 2;
		return (2);
	}
	path = getcwd(NULL, 0);
	if (!path)
		return (print_error("pwd"), 1);
	ft_printf("%s\n", path);
	free(path);
	return (0);
}

// NOTE : getcwd() always resolves symlinks to the physical path.
// To track the logical path (like bash does) we would need to build it manually:
//   - If path is absolute -> use it directly as new PWD
//   - If path is relative -> join current PWD + "/" + path,
//     then normalize . and .. without resolving symlinks
// This requires a path normalizer function. Would fix cd /bin -> /usr/bin issue.