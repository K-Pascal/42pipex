/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:08:55 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/17 18:08:35 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"

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
	close(fds[0]);
	close(fds[1]);
}

void	my_perror(char const name[], char const msg[])
{
	char *error_msg = ft_strjoin(name, msg);
	if (error_msg == NULL)
	{
		perror("my_perror():ft_strjoin()");
		return ;
	}
	ft_putstr_fd(error_msg, STDERR_FILENO);
	free(error_msg);
}
