/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:13:44 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/09 16:23:27 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include "libft/libft.h"

#include "parser.h"
#include "process.h"
#include "utils.h"

#define MSG1 "Usage : ./pipex [input file] [cmd 1] ... [cmd n] [output file]\n"
#define MSG2 "Usage : ./pipex here_doc [LIMITER] [cmd 1] [cmd 2] [output file]\n"

static int	init_data(t_data *data, int argc, char **argv, char **envp);
static void	split_process(t_data *data, int pipefd[2]);

int	main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	t_data	data;
	int		start;

	if (argc < 5)
	{
		ft_putstr_fd(MSG1 MSG2, 1);
		return (EXIT_FAILURE);
	}
	start = init_data(&data, argc, argv, envp);
	check_args(&data, &(argv[start]));
	if (pipe(pipefd) == -1)
	{
		perror("pipe()");
		return (EXIT_FAILURE);
	}
	split_process(&data, pipefd);
	my_n_free_all(data.cmds, data.nbr_cmds);
	return (EXIT_FAILURE);
}

static int	init_data(t_data *data, int argc, char **argv, char **envp)
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

static void	split_process(t_data *data, int pipefd[2])
{
	pid_t	fpid;

	fpid = fork();
	if (fpid == -1)
	{
		free_pipex(data, NULL, pipefd, "split_process():fork()");
		exit(EXIT_FAILURE);
	}
	if (fpid == 0)
		parent_process(data, pipefd);
	else
	{
		wait(NULL);
		child_process(data, pipefd);
	}
}
