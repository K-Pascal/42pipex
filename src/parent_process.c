/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:58:56 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/10 18:44:48 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft/libft.h"

#include "commands.h"
#include "utils.h"

static void	exec_inter_cmds(t_data *data, int pipefd[2]);
static void	create_other_process(t_data *data, int i, int pipefd[2], int fd_in);

void	parent_process(t_data *data, int pipefd[2])
{
	int		flags;
	char	**argv;

	if (close(pipefd[1]) == -1)
		perror("parent_process():close()");
	exec_inter_cmds(data, pipefd);
	redirect_pipefd(pipefd[0], STDIN_FILENO);
	flags = O_CREAT | O_WRONLY;
	if (data->limiter != NULL)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	if (access(data->f_out, F_OK) != -1 && access(data->f_out, W_OK) == -1)
	{
		perror(data->f_out);
		exit(EXIT_FAILURE);
	}
	pipefd[1] = open(data->f_out, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (pipefd[1] == -1)
	{
		perror("parent_process():open()");
		exit(EXIT_FAILURE);
	}
	redirect_pipefd(pipefd[1], STDOUT_FILENO);
	argv = ft_split(data->cmds[data->nbr_cmds - 1], ' ');
	if (!argv)
	{
		perror("parent_process():ft_split()");
		exit(EXIT_FAILURE);
	}
	exec_prog(argv, data->envp);
	my_free_all(argv);
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
		//if (i == data->nbr_cmds)
		//	wait(NULL);
	}
}

static void	create_other_process(t_data *data, int i, int pipefd[2], int fd_in)
{
	pid_t	fpid;
	char	**argv;

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
	argv = ft_split(data->cmds[i], ' ');
	if (!argv)
	{
		perror("create_other_process():ft_split()");
		exit(EXIT_FAILURE);
	}
	exec_prog(argv, data->envp);
	my_free_all(argv);
	exit(EXIT_FAILURE);
}
