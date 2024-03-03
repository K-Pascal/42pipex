/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:13:44 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/05 15:54:48 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include "libft/libft.h"
#include "gnl/get_next_line.h"

#undef BUFFER_SIZE
#define BUFFER_SIZE 1

#define MSG1 "Usage : ./pipex [input file] [cmd 1] ... [cmd n] [output file]\n"
#define MSG2 "Usage : ./pipex here_doc [LIMITER] [cmd 1] [cmd 2] [output file]\n"

typedef struct s_data
{
	char	*limiter;
	char	*file_input;
	char	*file_output;
	int		nbr_cmds;
	char	**cmds;
	char	**envp;
}	t_data;

void	exec_prog(char const *pathname, char *const argv[], char *const envp[])
{
	execve(pathname, argv, envp);
	perror(pathname);
}

void	my_free_all(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

void	my_n_free_all(char **arr, int len)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (i < len)
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

void	quit_prog(t_data *data, char **path, char msg[], int status)
{
	if (msg != NULL)
		perror(msg);
	if (data != NULL)
		my_n_free_all(data->cmds, data->nbr_cmds);
	my_free_all(path);
	exit(status);
}

char	*findpath(char **envp)
{
	int	i;

	if (envp == NULL)
		return (NULL);
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
	if (paths == NULL)
		return (NULL);
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
	perror(&(cmd_name[1]));
	free(cmd_name);
	return (NULL);
}

void	check_args(t_data *data, char **argv)
{
	int		i;
	char	*path_env;
	char	**paths;

	paths = NULL;
	if (data->file_input && access(data->file_input, F_OK | R_OK) == -1)
		quit_prog(data, NULL, data->file_input, EXIT_FAILURE);
	if (access(data->file_output, F_OK) != -1
		&& access(data->file_output, W_OK) == -1)
		quit_prog(data, NULL, data->file_output, EXIT_FAILURE);
	path_env = findpath(data->envp);
	if (path_env)
	{
		paths = ft_split(path_env, ':');
		if (!paths)
			quit_prog(data, NULL, "check_args():ft_split()", EXIT_FAILURE);
	}
	data->cmds = malloc(data->nbr_cmds * sizeof(char *));
	if (!data->cmds)
		quit_prog(data, paths, "check_args():malloc()", EXIT_FAILURE);
	i = 0;
	while (i < data->nbr_cmds)
	{
		data->cmds[i] = check_command(argv[i], paths);
		if (!data->cmds[i])
		{
			my_n_free_all(data->cmds, i);
			quit_prog(NULL, paths, NULL, EXIT_FAILURE);
		}
		i++;
	}
	my_free_all(paths);
}

void	read_input(int pipefd[2], t_data *data)
{
	size_t	len_limiter;
	char	*input;
	char	*limiter;

	if (close(pipefd[0]) == -1)
		perror("read_input():close()");
	limiter = ft_strjoin(data->limiter, "\n");
	if (!limiter)
		quit_prog(data, NULL, "read_input():ft_strjoin()", EXIT_FAILURE);
	len_limiter = ft_strlen(limiter);
	ft_putstr_fd("> ", STDIN_FILENO);
	input = get_next_line(STDIN_FILENO);
	while (input && ft_strncmp(input, limiter, len_limiter))
	{
		ft_putstr_fd(input, pipefd[1]);
		ft_putstr_fd("> ", STDIN_FILENO);
		free(input);
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
		quit_prog(data, NULL, "parent_process():dup2()", EXIT_FAILURE);
	if (close(pipefd[1]) == -1)
		perror("parent_process():close()");
	if (data->limiter != NULL)
	{
		if (pipe(pipefd) == -1)
			quit_prog(data, NULL, "parent_process():pipe()", EXIT_FAILURE);
		fpid = fork();
		if (fpid == -1)
			quit_prog(data, NULL, "parent_process():fork()", EXIT_FAILURE);
		else if (!fpid)
		{
			read_input(pipefd, data);
			return ;
		}
		if (close(pipefd[1]) == -1)
			perror("parent_process():close()");
		waitpid(0, NULL, WCONTINUED);
	}
	else
	{
		pipefd[0] = open(data->file_input, O_RDONLY);
		if (pipefd[0] == -1)
			quit_prog(data, NULL, "parent_process():open()", EXIT_FAILURE);
	}
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		quit_prog(data, NULL, "parent_process():dup2()", EXIT_FAILURE);
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
		data->file_input = NULL;
		data->nbr_cmds = argc - 4;
		start = 3;
	}
	else
	{
		data->file_input = argv[1];
		data->limiter = NULL;
		data->nbr_cmds = argc - 3;
		start = 2;
	}
	data->file_output = argv[argc - 1];
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
		perror("main():fork()");
		exit(EXIT_FAILURE);
	}
	if (fpid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		if (data.limiter != NULL)
			pipefd[1] = open(data.file_output, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		else
			pipefd[1] = open(data.file_output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		argv = ft_split(data.cmds[1], ' ');
		exec_prog(argv[0], argv, envp);
		my_free_all(argv);
	}
	else
	{
		parent_process(pipefd, &data);
		wait(NULL);
	}
	my_n_free_all(data.cmds, data.nbr_cmds);
	return (EXIT_SUCCESS);
}
