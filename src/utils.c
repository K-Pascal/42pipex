/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:08:55 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/09 18:25:12 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"

#include "commands.h"
#include "utils.h"

int	redirect_pipefd(int fd, int newfd)
{
	if (dup2(fd, newfd) == -1)
	{
		perror("redirect_pipefd():dup2()");
		close(fd);
		return (0);
	}

	if (close(fd) == -1)
		perror("redirect_pipefd():close()");
	return (1);
}

void	close_pipe(int fds[2])
{
	if (fds != NULL)
	{
		close(fds[0]);
		close(fds[1]);
	}
}

void	my_free_all(char **arr)
{
	if (arr == NULL)
		return ;

	int i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

void	my_perror(char const name[], char const msg[])
{
	char *error_msg = ft_strjoin(name, msg);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	free(error_msg);
}

void	prepare_command(t_data *data, int i)
{
	char **argv = ft_split(data->cmds[i], ' ');
	if (argv == NULL)
	{
		perror("prepare_command():ft_split()");
		exit(EXIT_FAILURE);
	}

	int status = exec_prog(data->cmds[i], argv, data->envp);
	my_free_all(argv);
	exit(status);
}
