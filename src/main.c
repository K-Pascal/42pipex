/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:13:44 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/08 17:37:15 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include "libft/libft.h"
#include "gnl/get_next_line.h"
#include "utils.h"
#include "parser.h"

#define MSG1 "Usage : ./pipex [input file] [cmd 1] ... [cmd n] [output file]\n"
#define MSG2 "Usage : ./pipex here_doc [LIMITER] [cmd 1] [cmd 2] [output file]\n"

void	read_input(int pipefd[2], t_data *data)
{
	size_t	len_limiter;
	char	*input;
	char	*limiter;

	if (close(pipefd[0]) == -1)
		perror("read_input():close()");
	limiter = ft_strjoin(data->limiter, "\n");
	if (!limiter)
	{
		free_pipex(data, NULL, NULL, "read_input():ft_strjoin()");
		close(pipefd[1]);
		exit(EXIT_FAILURE);
	}
	len_limiter = ft_strlen(limiter);
	ft_putstr_fd("> ", STDIN_FILENO);
	input = get_next_line(STDIN_FILENO);
	while (input && !!ft_strncmp(input, limiter, len_limiter))
	{
		ft_putstr_fd(input, pipefd[1]);
		free(input);
		ft_putstr_fd("> ", STDIN_FILENO);
		input = get_next_line(STDIN_FILENO);
	}
	if (close(pipefd[1]) == -1)
		perror("read_input():close()");
	free(input);
	free(limiter);
}

void	parent_process(int pipefd[2], t_data *data)
{
	char	**argv;
	pid_t	fpid;

	if (close(pipefd[0]) == -1)
		perror("parent_process():close()");
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		free_pipex(data, NULL, NULL, "parent_process():dup2()");
		close(pipefd[1]);
		exit(EXIT_FAILURE);
	}
	if (close(pipefd[1]) == -1)
		perror("parent_process():close()");
	if (data->limiter != NULL)
	{
		if (pipe(pipefd) == -1)
		{
			free_pipex(data, NULL, NULL, "parent_process():pipe()");
			exit(EXIT_FAILURE);
		}
		fpid = fork();
		if (fpid == -1)
		{
			free_pipex(data, NULL, pipefd, "parent_process():fork()");
			exit(EXIT_FAILURE);
		}
		else if (!fpid)
		{
			read_input(pipefd, data);
			return ;
		}
		wait(NULL);
		if (close(pipefd[1]) == -1)
			perror("parent_process():close()");
	}
	else
	{
		pipefd[0] = open(data->f_in, O_RDONLY);
		if (pipefd[0] == -1)
		{
			free_pipex(data, NULL, NULL, "parent_process():open()");
			exit(EXIT_FAILURE);
		}
	}
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		free_pipex(data, NULL, NULL, "parent_process():dup2()");
		close(pipefd[0]);
		exit(EXIT_FAILURE);
	}
	if (close(pipefd[0]) == -1)
		perror("parent_process():close()");
	argv = ft_split(data->cmds[0], ' ');
	exec_prog(argv[0], argv, data->envp);
	my_free_all(argv);
}

int	init_data(t_data *data, int argc, char **argv, char **envp)
{
	int	start;

	if (!ft_strncmp(argv[1], "here_doc", 8))
	{
		if (argc != 6)
		{
			ft_putendl_fd(MSG2, 2);
			exit(EXIT_FAILURE);
		}
		data->limiter = argv[2];
		data->f_in = NULL;
		data->nbr_cmds = argc - 4;
		start = 3;
	}
	else
	{
		data->f_in = argv[1];
		data->limiter = NULL;
		data->nbr_cmds = argc - 3;
		start = 2;
	}
	data->f_out = argv[argc - 1];
	data->cmds = NULL;
	data->envp = envp;
	return (start);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	fpid;
	int		pipefd[2];
	t_data	data;
	int		start;
	int		i;

	if (argc < 5)
	{
		ft_putstr_fd(MSG1 MSG2, 2);
		return (EXIT_FAILURE);
	}
	start = init_data(&data, argc, argv, envp);
	check_args(&data, &(argv[start]));
	if (pipe(pipefd) == -1)
	{
		perror("main():pipe()");
		return (EXIT_FAILURE);
	}
	fpid = fork();
	if (fpid == -1)
	{
		free_pipex(&data, NULL, pipefd, "main():fork()");
		exit(EXIT_FAILURE);
	}
	if (fpid == 0)
	{
		if (close(pipefd[1]) == -1)
			perror("close()");
		i = 1;
		while (i + 1 < data.nbr_cmds)
		{
			int	fds[2];
			if (pipe(fds) == -1)
			{
				free_pipex(&data, NULL, NULL, "pipe()");
				close(pipefd[0]);
				exit(EXIT_FAILURE);
			}
			fpid = fork();
			if (fpid == -1)
			{
				free_pipex(&data, NULL, fds, "fork()");
				close(pipefd[0]);
				exit(EXIT_FAILURE);
			}
			if (fpid == 0)
			{
				if (close(fds[0]) == -1)
					perror("close()");
				if (dup2(pipefd[0], STDIN_FILENO) == -1)
				{
					free_pipex(&data, NULL, NULL, "dup2()");
					close(pipefd[0]);
					close(fds[1]);
					exit(EXIT_FAILURE);
				}
				if (close(pipefd[0]) == -1)
					perror("close()");
				if (dup2(fds[1], STDOUT_FILENO) == -1)
				{
					free_pipex(&data, NULL, NULL, "dup2()");
					close(fds[1]);
					exit(EXIT_FAILURE);
				}
				if (close(fds[1]) == -1)
					perror("close()");
				argv = ft_split(data.cmds[i], ' ');
				exec_prog(argv[0], argv, envp);
				my_free_all(argv);
				my_n_free_all(data.cmds, data.nbr_cmds);
				return (EXIT_FAILURE);
			}
			wait(NULL);
			if (close(fds[1]) == -1)
				perror("close()");
			if (close(pipefd[0]) == -1)
				perror("close()");
			pipefd[0] = fds[0];
			i++;
		}
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			free_pipex(&data, NULL, NULL, "dup2()");
			close(pipefd[0]);
		}
		if (close(pipefd[0]) == -1)
			perror("close()");
		if (data.limiter != NULL)
			pipefd[1] = open(data.f_out, O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		else
			pipefd[1] = open(data.f_out, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (pipefd[1] == -1)
		{
			free_pipex(&data, NULL, NULL, "open()");
			exit(EXIT_FAILURE);
		}
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			free_pipex(&data, NULL, NULL, "dup2()");
			close(pipefd[1]);
			exit(EXIT_FAILURE);
		}
		if (close(pipefd[1]) == -1)
			perror("close()");
		argv = ft_split(data.cmds[i], ' ');
		exec_prog(argv[0], argv, envp);
		my_free_all(argv);
	}
	else
	{
		parent_process(pipefd, &data);
		wait(NULL);
	}
	my_n_free_all(data.cmds, data.nbr_cmds);
	return (EXIT_FAILURE);
}
