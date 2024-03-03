/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:13:44 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/03 19:22:20 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include "libft/libft.h"

typedef struct s_args
{
	int		argc;
	char	*delimiter;
	char	*file_input;
	char	*file_output;
	int		nbr_cmds;
	char	**cmds;
	char	**envp;
}	t_args;

void	exec_prog(char const *pathname, char *const argv[], char *const envp[])
{
	execve(pathname, argv, envp);
	perror(pathname);
}

void	my_free_all(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	my_n_free_all(char **arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*findpath(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

int	is_valid_command(char arg[])
{
	int	i;
	int	len;
	int	status;

	i = 0;
	while (arg[i] && arg[i] != ' ')
		i++;
	len = i;
	while (arg[len])
		len++;
	arg[i] = '\0';
	status = access(arg, F_OK | R_OK | X_OK) != -1;
	if (i != len)
		arg[i] = ' ';
	return (status);
}

char	*check_command(char cmd[], char **paths)
{
	char	*cmd_name;
	int		i;

	if (is_valid_command(cmd))
		return (ft_strdup(cmd));
	cmd_name = ft_strjoin("/", cmd);
	i = 0;
	while (paths[i])
	{
		cmd = ft_strjoin(paths[i], cmd_name);
		if (is_valid_command(cmd))
		{
			free(cmd_name);
			return (cmd);
		}
		free(cmd);
		i++;
	}
	perror(cmd_name + 1);
	free(cmd_name);
	return (NULL);
}

void	check_args(t_args *args, char **argv)
{
	int		i;
	char	*path_env;
	char	**paths;

	if (args->file_input && access(args->file_input, F_OK | R_OK) == -1)
	{
		perror(args->file_input);
		exit(EXIT_FAILURE);
	}
	if (access(args->file_output, F_OK) != -1
		&& access(args->file_output, W_OK) == -1)
	{
		perror(args->file_output);
		exit(EXIT_FAILURE);
	}
	path_env = findpath(args->envp);
	if (!path_env)
	{
		ft_putendl_fd("environment variable PATH not found", 2);
		exit(EXIT_FAILURE);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		perror("ft_split()");
		exit(EXIT_FAILURE);
	}
	args->cmds = malloc(args->nbr_cmds * sizeof(char *));
	if (!args->cmds)
	{
		perror("check_args()");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < args->nbr_cmds)
	{
		args->cmds[i] = check_command(argv[i], paths);
		if (!args->cmds[i])
		{
			my_n_free_all(args->cmds, i);
			my_free_all(paths);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	my_free_all(paths);
}

void	parent(int pipefd[2], char file[], char command[], char **envp)
{
	char	**argv;

	dup2(pipefd[1], 1);
	if (close(pipefd[0]) == -1|| close(pipefd[1]) == -1)
	{
		perror("close");
		return ;
	}
	pipefd[0] = open(file, O_RDONLY);
	dup2(pipefd[0], 0);
	close(pipefd[0]);
	argv = ft_split(command, ' ');
	exec_prog(argv[0], argv, envp);
	my_free_all(argv);
	wait(NULL);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	fpid;
	int		pipefd[2];
	t_args	args;
	int		start;

	if (argc < 5)
		return (EXIT_FAILURE);
	if (!ft_strncmp(argv[1], "here_doc", 8))
	{
		if (argc != 6)
		{
			ft_putendl_fd("Usage : ./pipex here_doc [LIMITER] [cmd1] [cmd2] [output file]", 2);
			return (EXIT_FAILURE);
		}
		args.delimiter = argv[2];
		args.file_input = NULL;
		args.nbr_cmds = argc - 4;
		start = 3;
	}
	else
	{
		args.file_input = argv[1];
		args.delimiter = NULL;
		args.nbr_cmds = argc - 3;
		start = 2;
	}
	args.argc = argc;
	args.file_output = argv[argc - 1];
	args.envp = envp;
	check_args(&args, &(argv[start]));
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	fpid = fork();
	if (fpid == -1)
	{
		if (close(pipefd[0]) == -1)
			perror("close");
		if (close(pipefd[1]) == -1)
			perror("close");
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (fpid == 0)
	{
		close(pipefd[1]);

		dup2(pipefd[0], 0);
		close(pipefd[0]);
		pipefd[1] = open(args.file_output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		// read
		argv = ft_split(args.cmds[1], ' ');
		exec_prog(argv[0], argv, envp);
		my_free_all(argv);
	}
	else
	{
		parent(pipefd, args.file_input, args.cmds[0], args.envp);
	}
	my_n_free_all(args.cmds, args.nbr_cmds);
	return (EXIT_SUCCESS);
}
