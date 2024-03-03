/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:58:36 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/03 18:52:05 by pnguyen-         ###   ########.fr       */
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
static char	*joinpath(char path[], char cmd[]);

int	exec_prog(char **argv, char **envp)
{
	char	**paths = get_path(envp);
	char	*pathname = check_command(argv[0], paths, F_OK | X_OK);
	if (pathname == NULL)
		pathname = check_command(argv[0], paths, F_OK);
	my_free_all(paths);
	if (pathname == NULL)
	{
		if (ft_strncmp(argv[0], "./", 2) != 0)
		{
			ft_putstr_fd(argv[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
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
	if (envp == NULL)
		return (NULL);

	char	*path_env = NULL;
	int		i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (path_env == NULL)
		return (NULL);

	char **paths = ft_split(path_env, ':');
	if (paths == NULL)
		perror("getpath():ft_split()");

	return (paths);
}

static char	*check_command(char cmd_name[], char **paths, int mode)
{
	if (access(cmd_name, mode) != -1)
		return (ft_strdup(cmd_name));

	if (paths == NULL || ft_strncmp(cmd_name, "./", 2) == 0)
		return (NULL);

	int i = 0;
	while (paths[i] != NULL)
	{
		char *cmd = joinpath(paths[i], cmd_name);
		if (access(cmd, mode) != -1)
			return (cmd);
		free(cmd);
		i++;
	}

	return (NULL);
}

static char	*joinpath(char path[], char cmd[])
{
	size_t	len_path = ft_strlen(path);
	size_t	len_cmd = ft_strlen(cmd);
	char	*pathname = malloc((len_path + len_cmd + 2) * sizeof(char));
	if (pathname == NULL)
		return (NULL);

	ft_strlcpy(pathname, path, len_path + 1);
	pathname[len_path] = '/';
	ft_strlcpy(pathname + len_path + 1, cmd, len_cmd + 1);
	return (pathname);
}
