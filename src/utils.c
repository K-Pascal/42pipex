/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:08:55 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/10 18:50:30 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
