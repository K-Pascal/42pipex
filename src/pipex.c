/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:58:56 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/15 21:01:39 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft/libft.h"

#include "utils.h"
#include "process.h"

static pid_t	create_process(t_data *data, int index, int fds[2], int fd_in);
static void		pipe_exec(t_data *data, int i, int fds[2], int fd_in);
static int		final_cmd(t_data *data, int fd_in);
static void		wait_all(pid_t last_process_id, int *status);

void	pipex(t_data *data, int *status)
{
	int		i;
	int		pipefd[2];
	int		fd_in;
	pid_t	fpid;

	fd_in = -1;
	i = 0;
	while (i < data->nbr_cmds)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipex():pipe()");
			close(fd_in);
			exit(EXIT_FAILURE);
		}
		fpid = create_process(data, i, pipefd, fd_in);
		fd_in = pipefd[0];
		i++;
	}
	if (close(fd_in) == -1)
		perror("pipex():close(fd_in)");
	wait_all(fpid, status);
}

static pid_t	create_process(t_data *data, int index, int fds[2], int fd_in)
{
	pid_t	fpid;

	fpid = fork();
	if (fpid == -1)
	{
		perror("create_process():fork()");
		close_pipe(fds);
		close(fd_in);
		exit(EXIT_FAILURE);
	}
	if (fpid == 0)
		pipe_exec(data, index, fds, fd_in);
	if (close(fds[1]) == -1)
		perror("create_process():close(fds[1])");
	if (fd_in != -1 && close(fd_in) == -1)
		perror("create_process():close(fd_in)");
	return (fpid);
}

static void	pipe_exec(t_data *data, int index, int fds[2], int fd_in)
{
	if (close(fds[0]) == -1)
		perror("pipex_exec():close(fds[0])");
	if (index + 1 == data->nbr_cmds)
	{
		if (close(fds[1]) == -1)
			perror("pipe_exec():close(fds[1])");
		fds[1] = final_cmd(data, fd_in);
	}
	redirect_pipefd(fds[1], STDOUT_FILENO);
	if (index == 0)
		fd_in = choose_input(data);
	redirect_pipefd(fd_in, STDIN_FILENO);
	prepare_command(data, index);
}

static int	final_cmd(t_data *data, int fd_in)
{
	int	flags;
	int	fd_out;

	if (access(data->f_out, F_OK) != -1 && access(data->f_out, W_OK) == -1)
	{
		perror(data->f_out);
		close(fd_in);
		exit(EXIT_FAILURE);
	}
	flags = O_CREAT | O_WRONLY;
	if (data->limiter != NULL)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd_out = open(data->f_out, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd_out == -1)
	{
		perror("final_cmd():open()");
		close(fd_in);
		exit(EXIT_FAILURE);
	}
	return (fd_out);
}

static void	wait_all(pid_t last_process_id, int *status)
{
	pid_t	fpid;

	fpid = waitpid(-1, status, WNOHANG);
	while (fpid != last_process_id && fpid != -1)
		fpid = waitpid(-1, status, WNOHANG);
	while (waitpid(-1, NULL, WNOHANG) >= 0)
		;
}
