/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:58:36 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/09 15:08:51 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "libft/libft.h"

#include "utils.h"

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
	status = access(arg, F_OK | R_OK | X_OK) != -1;
	if (i != len)
		arg[i] = ' ';
	return (status);
}

static char	*check_command(char cmd[], char **paths)
{
	char	*cmd_name;
	int		i;

	if (is_valid_command(cmd))
		return (ft_strdup(cmd));
	if (paths == NULL)
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
	perror(cmd_name + 1);
	free(cmd_name);
	return (NULL);
}

static void	verify_list_commands(t_data *data, char **paths, char **argv)
{
	int		i;

	data->cmds = malloc(data->nbr_cmds * sizeof(char *));
	if (!data->cmds)
	{
		free_pipex(NULL, paths, NULL, "check_args():malloc()");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < data->nbr_cmds)
	{
		data->cmds[i] = check_command(argv[i], paths);
		if (!data->cmds[i])
		{
			my_n_free_all(data->cmds, i);
			free_pipex(NULL, paths, NULL, NULL);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	my_free_all(paths);
}

void	check_args(t_data *data, char **argv)
{
	char	*path_env;
	char	**paths;

	if (data->f_in && access(data->f_in, F_OK | R_OK) == -1)
	{
		free_pipex(NULL, NULL, NULL, data->f_in);
		exit(EXIT_FAILURE);
	}
	if (access(data->f_out, F_OK) != -1 && access(data->f_out, W_OK) == -1)
	{
		free_pipex(NULL, NULL, NULL, data->f_out);
		exit(EXIT_FAILURE);
	}
	paths = NULL;
	path_env = findpath(data->envp);
	if (path_env)
	{
		paths = ft_split(path_env, ':');
		if (!paths)
		{
			free_pipex(NULL, NULL, NULL, "check_args():ft_split()");
			exit(EXIT_FAILURE);
		}
	}
	verify_list_commands(data, paths, argv);
}
