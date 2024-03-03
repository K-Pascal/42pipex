/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:44:22 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/12 16:26:02 by pnguyen-         ###   ########.fr       */
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

static int	do_heredoc(t_data *data, int fds[2]);
static void	read_input(t_data *data, int pipefd[2]);
static char	*ask_input(void);

//void	child_process(t_data *data, int pipefd[2])
//{
//	if (close(pipefd[0]) == -1)
//		perror("child_process():close(pipefd[0])");
//	redirect_pipefd(pipefd[1], STDOUT_FILENO);
//	choose_input(data, pipefd);
//	redirect_pipefd(pipefd[0], STDIN_FILENO);
//	prepare_command(data, 0);
//}

int	choose_input(t_data *data, int fds[2])
{
	int	fd_in;

	if (data->limiter != NULL)
		fd_in = do_heredoc(data, fds);
	else
	{
		if (access(data->f_in, F_OK | R_OK) == -1)
		{
			perror(data->f_in);
			close_pipe(fds);
			exit(EXIT_FAILURE);
		}
		fd_in = open(data->f_in, O_RDONLY);
		if (fd_in == -1)
		{
			perror("child_process():open()");
			close_pipe(fds);
			exit(EXIT_FAILURE);
		}
	}
	return (fd_in);
}

static int	do_heredoc(t_data *data, int fds[2])
{
	pid_t	fpid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("do_heredoc():pipe()");
		close_pipe(fds);
		exit(EXIT_FAILURE);
	}
	fpid = fork();
	if (fpid == -1)
	{
		perror("do_heredoc():fork()");
		close_pipe(pipefd);
		close_pipe(fds);
		exit(EXIT_FAILURE);
	}
	if (fpid == 0)
	{
		close_pipe(fds);
		read_input(data, pipefd);
		exit(EXIT_SUCCESS);
	}
	waitpid(0, NULL, WUNTRACED);
	if (close(pipefd[1]) == -1)
		perror("do_heredoc():close()");
	return (pipefd[0]);
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
