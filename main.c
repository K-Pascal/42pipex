/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:13:44 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/09 17:07:39 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "libft/libft.h"

#include "process.h"

#define MSG "Usage: ./pipex [input file] [cmd 1] [cmd 2] [output file]\n"
#define SIG_RETURN 128

static void	init_data(t_data *data, int argc, char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		ft_putstr_fd(MSG, STDERR_FILENO);
		return (EXIT_FAILURE);
	}

	t_data	data;
	init_data(&data, argc, argv, envp);

	int		status = 0;
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
	data->f_in = argv[1];
	data->limiter = NULL;
	data->nbr_cmds = 2;
	data->f_out = argv[argc - 1];
	data->cmds = &argv[2];
	data->envp = envp;
}
