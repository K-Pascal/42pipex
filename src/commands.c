/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:58:36 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/17 17:55:57 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"

#include "pipex.h"
#include "utils.h"

#define ERR_NOT_FOUND 127
#define ERR_ACCESS 126
#define NOT_FOUND_MSG ": command not found\n"

static int	exec_prog(char const full_cmd[], char **argv, char **envp);
static char	**get_path(char **envp);
static char	*check_command(char cmd[], char **paths, int mode);
static char	*join_path(char path[], char cmd[]);

void	prepare_command(t_data *data, int i)
{
	char **argv = ft_split(data->cmds[i], ' ');
	if (argv == NULL)
	{
		perror("prepare_command():ft_split()");
		exit(EXIT_FAILURE);
	}

	int status = exec_prog(data->cmds[i], argv, data->envp);
	ft_free_all(argv);
	exit(status);
}

static int	exec_prog(char const full_cmd[], char **argv, char **envp)
{
	if (argv[0] == NULL)
	{
		my_perror(full_cmd, NOT_FOUND_MSG);
		return (ERR_NOT_FOUND);
	}

	char	**paths = get_path(envp);
	char	*pathname = check_command(argv[0], paths, F_OK | X_OK);
	if (pathname == NULL)
		pathname = check_command(argv[0], paths, F_OK);
	if (paths != NULL)
		ft_free_all(paths);
	if (pathname == NULL)
	{
		if (ft_strchr(argv[0], '/') != NULL)
			perror(argv[0]);
		else
			my_perror(argv[0], NOT_FOUND_MSG);
		return (ERR_NOT_FOUND);
	}

	execve(pathname, argv, envp);
	int errsv = errno;
	perror(pathname);
	free(pathname);
	return (errsv == EACCES ? ERR_ACCESS : EXIT_FAILURE);
}

static char	**get_path(char **envp)
{
	if (envp == NULL)
		return (NULL);

	int		i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		++i;
	if (envp[i] == NULL)
		return (NULL);

	char **paths = ft_split(envp[i] + 5, ':');
	if (paths == NULL)
		perror("get_path():ft_split()");

	return (paths);
}

static char	*check_command(char cmd_name[], char **paths, int mode)
{
	if (ft_strchr(cmd_name, '/') != NULL)
	{
		if (access(cmd_name, mode) != -1)
			return (ft_strdup(cmd_name));
		else
			return (NULL);
	}

	if (paths == NULL)
		return (NULL);

	int i = 0;
	while (paths[i] != NULL)
	{
		char *cmd = join_path(paths[i], cmd_name);
		if (cmd == NULL)
			return (NULL);
		if (access(cmd, mode) != -1)
			return (cmd);
		free(cmd);
		++i;
	}

	return (NULL);
}

static char	*join_path(char path[], char cmd[])
{
	size_t	len_path = ft_strlen(path);
	size_t	len_cmd = ft_strlen(cmd);
	char	*pathname = malloc((len_path + len_cmd + 2) * sizeof(char));
	if (pathname == NULL)
	{
		perror("join_path():malloc()");
		return (NULL);
	}

	ft_memcpy(pathname, path, len_path);
	pathname[len_path] = '/';
	ft_memcpy(pathname + len_path + 1, cmd, len_cmd);
	pathname[len_path + 1 + len_cmd] = '\0';
	return (pathname);
}
