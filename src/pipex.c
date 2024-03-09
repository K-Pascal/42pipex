/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:58:56 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/09 19:16:35 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft/libft.h"

#include "pipex.h"
#include "input_output.h"
#include "utils.h"

static pid_t	create_process(t_data *data, int index, int fds[2], int fd_in);
static void		pipe_exec(t_data *data, int i, int fds[2], int fd_in);
static void		wait_all(pid_t last_process_id, int *status);

void	pipex(t_data *data, int *status)
{
	pid_t	fpid;
	int		fd_in = -1;
	int		i = 0;
	while (i < data->nbr_cmds)
	{
		int pipefd[2];
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
	pid_t fpid = fork();
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
		fds[1] = open_outfile(data);
	}
	if (fds[1] == -1 || !redirect_pipefd(fds[1], STDOUT_FILENO))
	{
		close(fd_in);
		exit(EXIT_FAILURE);
	}

	if (index == 0)
		fd_in = open_infile(data);

	if (fd_in == -1 || !redirect_pipefd(fd_in, STDIN_FILENO))
		exit(EXIT_FAILURE);

	prepare_command(data, index);
}

static void	wait_all(pid_t last_process_id, int *status)
{
	pid_t fpid = waitpid(0, status, WNOHANG | WUNTRACED);
	while (fpid != last_process_id && fpid != -1)
		fpid = waitpid(0, status, WNOHANG | WUNTRACED);
	while (waitpid(0, NULL, WNOHANG) >= 0)
		;
}
