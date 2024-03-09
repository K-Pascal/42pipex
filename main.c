/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:13:44 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/09 20:34:30 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "libft/libft.h"

#include "pipex.h"

#define MSG1 "Usage: ./pipex [input file] [cmd 1] ... [cmd n] [output file]\n"

#define MSG2 "Usage: ./pipex here_doc [LIMITER] [cmd 1] ... \
[cmd n] [output file]\n"

#define SIG_RETURN 128

static void	init_data(t_data *data, int argc, char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	if (argc < 5)
	{
		ft_putstr_fd(MSG1 MSG2, STDERR_FILENO);
		return (EXIT_FAILURE);
	}

	t_data	data;
	init_data(&data, argc, argv, envp);

	int		status = 0;
	pipex(&data, &status);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (SIG_RETURN + WTERMSIG(status));
	else
		return (EXIT_FAILURE);
}

static void	init_data(t_data *data, int argc, char **argv, char **envp)
{
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
		{
			ft_putendl_fd(MSG2, STDERR_FILENO);
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
