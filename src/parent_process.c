/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:58:56 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/11 18:38:06 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft/libft.h"

#include "utils.h"

static void	exec_inter_cmds(t_data *data, int pipefd[2]);
static void	create_other_process(t_data *data, int i, int pipefd[2], int fd_in);
static void	exec_final_cmd(t_data *data, int fd);

void	parent_process(t_data *data, int pipefd[2])
{
	int		flags;

	if (close(pipefd[1]) == -1)
		perror("parent_process():close()");
	exec_inter_cmds(data, pipefd);
	redirect_pipefd(pipefd[0], STDIN_FILENO);
	if (access(data->f_out, F_OK) != -1 && access(data->f_out, W_OK) == -1)
	{
		perror(data->f_out);
		exit(EXIT_FAILURE);
	}
	flags = O_CREAT | O_WRONLY;
	if (data->limiter != NULL)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	exec_final_cmd(data, flags);
	while (waitpid(-1, NULL, WUNTRACED) > 0)
		;
}

static void	exec_inter_cmds(t_data *data, int pipefd[2])
{
	int	fds[2];
	int	i;

	i = 1;
	while (i + 1 < data->nbr_cmds)
	{
		if (pipe(fds) == -1)
		{
			perror("exec_inter_cmds():pipe()");
			close(pipefd[0]);
			exit(EXIT_FAILURE);
		}
		create_other_process(data, i, fds, pipefd[0]);
		if (close(fds[1]) == -1)
			perror("close()");
		if (close(pipefd[0]) == -1)
			perror("close()");
		pipefd[0] = fds[0];
		i++;
//		waitpid(0, NULL, WNOHANG);
	}
}

static void	create_other_process(t_data *data, int i, int pipefd[2], int fd_in)
{
	pid_t	fpid;

	fpid = fork();
	if (fpid == -1)
	{
		perror("create_other_process():fork()");
		close_pipe(pipefd);
		close(fd_in);
		exit(EXIT_FAILURE);
	}
	if (fpid != 0)
		return ;
	if (close(pipefd[0]) == -1)
		perror("create_other_process():close()");
	redirect_pipefd(fd_in, STDIN_FILENO);
	redirect_pipefd(pipefd[1], STDOUT_FILENO);
	prepare_command(data, i);
	exit(EXIT_FAILURE);
}

static void	exec_final_cmd(t_data *data, int flags)
{
	pid_t	fpid;
	int		fd;

	fpid = fork();
	if (fpid == -1)
	{
		perror("exec_final_cmd():fork()");
		exit(EXIT_FAILURE);
	}
	if (fpid == 0)
	{
		fd = open(data->f_out, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == -1)
		{
			perror("exec_final_cmd():open()");
			exit(EXIT_FAILURE);
		}
		redirect_pipefd(fd, STDOUT_FILENO);
		prepare_command(data, data->nbr_cmds - 1);
		exit(EXIT_FAILURE);
	}
}
