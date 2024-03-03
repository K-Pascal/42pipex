/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:44:22 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/17 17:24:50 by pnguyen-         ###   ########.fr       */
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

static int	do_heredoc(t_data *data);
static void	read_input(t_data *data, int pipefd[2]);
static char	*ask_input(void);

int	choose_input(t_data *data)
{
	int	fd_in;

	if (data->limiter != NULL)
		fd_in = do_heredoc(data);
	else
	{
		if (access(data->f_in, F_OK | R_OK) == -1)
		{
			perror(data->f_in);
			exit(EXIT_FAILURE);
		}
		fd_in = open(data->f_in, O_RDONLY);
		if (fd_in == -1)
		{
			perror(data->f_in);
			exit(EXIT_FAILURE);
		}
	}
	return (fd_in);
}

static int	do_heredoc(t_data *data)
{
	pid_t	fpid;
	int		pipefd[2];

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
	if (waitpid(fpid, NULL, WUNTRACED) == -1)
		perror("do_heredoc():waitpid()");
	if (close(pipefd[1]) == -1)
		perror("do_heredoc():close()");
	return (pipefd[0]);
}

static void	read_input(t_data *data, int pipefd[2])
{
	size_t	len_limiter;
	char	*input;

	if (close(pipefd[0]) == -1)
		perror("read_input():close(pipefd[0])");
	len_limiter = ft_strlen(data->limiter);
	input = ask_input();
	while (input != NULL)
	{
		if (!ft_strncmp(input, data->limiter, len_limiter)
			&& input[len_limiter] == '\n')
			break ;
		ft_putstr_fd(input, pipefd[1]);
		free(input);
		input = ask_input();
	}
	free(input);
	if (close(pipefd[1]) == -1)
		perror("read_input():close(pipefd[1])");
}

static char	*ask_input(void)
{
	char	*input;

	ft_putstr_fd("> ", STDIN_FILENO);
	input = get_next_line(STDIN_FILENO);
	return (input);
}
