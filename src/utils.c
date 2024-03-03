/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:08:55 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/09 15:04:35 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

void	exec_prog(char const *pathname, char *const argv[], char *const envp[])
{
	execve(pathname, argv, envp);
	perror(pathname);
}

void	redirect_pipefd(t_data *data, int fd, int newfd)
{
	if (dup2(fd, newfd) == -1)
	{
		free_pipex(data, NULL, NULL, "redirect_pipefd():dup2()");
		close(fd);
		exit(EXIT_FAILURE);
	}
	if (close(fd) == -1)
		perror("redirect_pipefd():close()");
}

void	free_pipex(t_data *data, char **path, int fds[2], char msg[])
{
	if (msg != NULL)
		perror(msg);
	if (data != NULL)
		my_n_free_all(data->cmds, data->nbr_cmds);
	if (path != NULL)
		my_free_all(path);
	if (fds != NULL)
	{
		close(fds[0]);
		close(fds[1]);
	}
}

void	my_free_all(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

void	my_n_free_all(char **arr, int len)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (i < len)
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}
