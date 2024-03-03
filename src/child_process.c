/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:44:22 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/10 19:00:52 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "gnl/get_next_line.h"
#include "libft/libft.h"

#include "utils.h"

static void	choose_input(t_data *data, int pipefd[2]);
static void	do_heredoc(t_data *data, int pipefd[2]);
static void	read_input(t_data *data, int pipefd[2]);
static char	*ask_input(void);

void	child_process(t_data *data, int pipefd[2])
{
	if (close(pipefd[0]) == -1)
		perror("child_process():close(pipefd[0])");
	redirect_pipefd(pipefd[1], STDOUT_FILENO);
	choose_input(data, pipefd);
	redirect_pipefd(pipefd[0], STDIN_FILENO);
	prepare_command(data, 0);
}

static void	choose_input(t_data *data, int pipefd[2])
{
	if (data->limiter != NULL)
		do_heredoc(data, pipefd);
	else
	{
		if (access(data->f_in, F_OK | R_OK) == -1)
		{
			perror(data->f_in);
			exit(EXIT_FAILURE);
		}
		pipefd[0] = open(data->f_in, O_RDONLY);
		if (pipefd[0] == -1)
		{
			perror("child_process():open()");
			exit(EXIT_FAILURE);
		}
	}
}

static void	do_heredoc(t_data *data, int pipefd[2])
{
	pid_t	fpid;

	if (pipe(pipefd) == -1)
	{
		perror("do_heredoc():pipe()");
		exit(EXIT_FAILURE);
	}
	fpid = fork();
	if (fpid == -1)
	{
		perror("do_heredoc():fork()");
		close_pipe(pipefd);
		exit(EXIT_FAILURE);
	}
	if (fpid == 0)
	{
		read_input(data, pipefd);
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
	if (close(pipefd[1]) == -1)
		perror("do_heredoc():close()");
}

static void	read_input(t_data *data, int pipefd[2])
{
	size_t	len_limiter;
	char	*input;
	char	*limiter;

	if (close(pipefd[0]) == -1)
		perror("read_input():close(pipefd[0])");
	limiter = ft_strjoin(data->limiter, "\n");
	if (!limiter)
	{
		perror("read_input():ft_strjoin()");
		close(pipefd[1]);
		exit(EXIT_FAILURE);
	}
	len_limiter = ft_strlen(limiter);
	input = ask_input();
	while (input && !!ft_strncmp(input, limiter, len_limiter))
	{
		ft_putstr_fd(input, pipefd[1]);
		free(input);
		input = ask_input();
	}
	if (close(pipefd[1]) == -1)
		perror("read_input():close(pipefd[1])");
	free(input);
	free(limiter);
}

static char	*ask_input(void)
{
	char	*input;

	ft_putstr_fd("> ", STDIN_FILENO);
	input = get_next_line(STDIN_FILENO);
	return (input);
}
