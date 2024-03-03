/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:13:44 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/18 15:22:28 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include "libft/libft.h"

#include "process.h"
#include "utils.h"

#define MSG1 "Usage: ./pipex [input file] [cmd 1] ... [cmd n] [output file]\n"
#define MSG2 "Usage: ./pipex here_doc [LIMITER] [cmd 1] [cmd 2] [output file]\n"
#define SIG_RETURN 128

static void	init_data(t_data *data, int argc, char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		status;

	if (argc < 5)
	{
		ft_putstr_fd(MSG1 MSG2, STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	init_data(&data, argc, argv, envp);
	status = 0;
	pipex(&data, &status);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = SIG_RETURN + WTERMSIG(status);
	else
		status = EXIT_FAILURE;
	return (status);
}

static void	init_data(t_data *data, int argc, char **argv, char **envp)
{
	if (!ft_strncmp(argv[1], "here_doc", 9))
	{
		if (argc != 6)
		{
			ft_putendl_fd(MSG2, STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
		data->limiter = argv[2];
		data->f_in = NULL;
		data->nbr_cmds = argc - 4;
		data->cmds = &argv[3];
	}
	else
	{
		data->f_in = argv[1];
		data->limiter = NULL;
		data->nbr_cmds = argc - 3;
		data->cmds = &argv[2];
	}
	data->f_out = argv[argc - 1];
	data->envp = envp;
}
