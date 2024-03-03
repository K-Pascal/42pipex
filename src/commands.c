/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:58:36 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/12 18:24:06 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "libft/libft.h"

#include "utils.h"

static char	*findpath(char **envp);
static char	*check_command(char cmd[], char **paths);
static int	is_valid_command(char arg[]);

int	exec_prog(char **argv, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*pathname;

	paths = NULL;
	path_env = findpath(envp);
	if (path_env)
	{
		paths = ft_split(path_env, ':');
		if (!paths)
		{
			perror("exec_prog():ft_split()");
			return (EXIT_FAILURE);
		}
	}
	pathname = check_command(argv[0], paths);
	my_free_all(paths);
	if (!pathname)
	{
		if (ft_strncmp(argv[0], "./", 2))
		{
			ft_putstr_fd(argv[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
		else
			perror(argv[0]);
		return (127);
	}
	execve(pathname, argv, envp);
	perror(pathname);
	free(pathname);
	return (EXIT_FAILURE);
}

static char	*findpath(char **envp)
{
	int	i;

	if (envp == NULL)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*check_command(char cmd[], char **paths)
{
	char	*cmd_name;
	int		i;

	if (is_valid_command(cmd))
		return (ft_strdup(cmd));
	if (paths == NULL)
		return (NULL);
	if (!ft_strncmp(cmd, "./", 2))
		return (NULL);
	cmd_name = ft_strjoin("/", cmd);
	i = 0;
	while (paths[i])
	{
		cmd = ft_strjoin(paths[i], cmd_name);
		if (is_valid_command(cmd))
		{
			free(cmd_name);
			return (cmd);
		}
		free(cmd);
		i++;
	}
	free(cmd_name);
	return (NULL);
}

static int	is_valid_command(char arg[])
{
	int	i;
	int	len;
	int	status;

	i = 0;
	while (arg[i] && arg[i] != ' ')
		i++;
	len = i;
	while (arg[len])
		len++;
	arg[i] = '\0';
	status = access(arg, F_OK | X_OK) != -1;
	if (i != len)
		arg[i] = ' ';
	return (status);
}
