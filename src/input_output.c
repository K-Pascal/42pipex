/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:44:22 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/05/07 17:57:18 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft/libft.h"

#include "pipex.h"
#include "utils.h"

#define HERE_DOC_NULL_MSG "\nwarning: here-document delimited by end-of-file "

static int	do_heredoc(t_data *data);
static void	read_input(t_data *data, int pipefd[2]);
static char	*ask_input(void);

int	open_infile(t_data *data)
{
	int fd_in;

	if (data->limiter != NULL)
		fd_in = do_heredoc(data);
	else
	{
		fd_in = open(data->f_in, O_RDONLY);
		if (fd_in == -1)
			goto fail;
	}

	return (fd_in);

fail:
	perror(data->f_in);
	return (-1);
}

int	open_outfile(t_data *data)
{
	int flags = O_CREAT | O_WRONLY;
	if (data->limiter != NULL)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;

	int fd_out = open(data->f_out, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd_out == -1)
		goto fail;

	return (fd_out);

fail:
	perror(data->f_out);
	return (-1);
}

static int	do_heredoc(t_data *data)
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("do_heredoc():pipe()");
		return (-1);
	}

	pid_t fpid = fork();
	if (fpid == -1)
	{
		perror("do_heredoc():fork()");
		close_pipe(pipefd);
		return (-1);
	}
	if (fpid == 0)
	{
		read_input(data, pipefd);
		exit(EXIT_SUCCESS);
	}

	if (close(pipefd[1]) == -1)
		perror("do_heredoc():close()");

	if (waitpid(fpid, NULL, 0) == -1)
		perror("do_heredoc():waitpid()");

	return (pipefd[0]);
}

static void	read_input(t_data *data, int pipefd[2])
{
	if (close(pipefd[0]) == -1)
		perror("read_input():close(pipefd[0])");

	size_t	len_limiter = ft_strlen(data->limiter);
	char	*input = ask_input();
	while (input != NULL)
	{
		if (ft_strncmp(input, data->limiter, len_limiter) == 0
				&& input[len_limiter] == '\n')
			break ;

		ft_putstr_fd(input, pipefd[1]);
		free(input);
		input = ask_input();
	}
	if (input == NULL)
	{
		write(STDERR_FILENO, HERE_DOC_NULL_MSG, ft_strlen(HERE_DOC_NULL_MSG));
		write(STDERR_FILENO, "(wanted '", 9);
		write(STDERR_FILENO, data->limiter, len_limiter);
		write(STDERR_FILENO, "')\n", 3);
	}
	free(input);

	if (close(pipefd[1]) == -1)
		perror("read_input():close(pipefd[1])");
}

static char	*ask_input(void)
{
	ft_putstr_fd("> ", STDERR_FILENO);
	return (get_next_line(STDIN_FILENO));
}
