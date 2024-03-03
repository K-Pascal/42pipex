/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:08:55 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/10 19:00:37 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"

#include "commands.h"
#include "utils.h"

void	redirect_pipefd(int fd, int newfd)
{
	if (dup2(fd, newfd) == -1)
	{
		perror("redirect_pipefd():dup2()");
		close(fd);
		exit(EXIT_FAILURE);
	}
	if (close(fd) == -1)
		perror("redirect_pipefd():close()");
}

void	close_pipe(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
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

void	prepare_command(t_data *data, int i)
{
	char	**argv;

	argv = ft_split(data->cmds[i], ' ');
	if (!argv)
	{
		perror("prepare_command():ft_split()");
		exit(EXIT_FAILURE);
	}
	exec_prog(argv, data->envp);
	my_free_all(argv);
}