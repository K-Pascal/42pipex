/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:58:36 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/16 14:46:48 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "libft/libft.h"

#include "utils.h"

#define ERR_CMD_NOT_FOUND 127

static char	**get_path(char **envp);
static char	*check_command(char cmd[], char **paths, int mode);
static int	is_valid_command(char arg[], int mode);

int	exec_prog(char **argv, char **envp)
{
	char	**paths;
	char	*pathname;

	paths = get_path(envp);
	pathname = check_command(argv[0], paths, F_OK | X_OK);
	if (!pathname)
		pathname = check_command(argv[0], paths, F_OK);
	my_free_all(paths);
	if (!pathname)
	{
		if (!!ft_strncmp(argv[0], "./", 2))
		{
			ft_putstr_fd(argv[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
		else
			perror(argv[0]);
		return (ERR_CMD_NOT_FOUND);
	}
	execve(pathname, argv, envp);
	perror(pathname);
	free(pathname);
	return (EXIT_FAILURE);
}

static char	**get_path(char **envp)
{
	char	*path_env;
	char	**paths;
	int		i;

	if (envp == NULL)
		return (NULL);
	path_env = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (path_env == NULL)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		perror("getpath():ft_split()");
	return (paths);
}

static char	*check_command(char cmd[], char **paths, int mode)
{
	char	*cmd_name;
	int		i;

	if (is_valid_command(cmd, mode))
		return (ft_strdup(cmd));
	if (paths == NULL || !ft_strncmp(cmd, "./", 2))
		return (NULL);
	cmd_name = ft_strjoin("/", cmd);
	i = 0;
	while (paths[i])
	{
		cmd = ft_strjoin(paths[i], cmd_name);
		if (is_valid_command(cmd, mode))
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

static int	is_valid_command(char arg[], int mode)
{
	int		i;
	char	temp;
	int		status;

	i = 0;
	while (arg[i] && arg[i] != ' ')
		i++;
	temp = arg[i];
	arg[i] = '\0';
	status = access(arg, mode) != -1;
	arg[i] = temp;
	return (status);
}
