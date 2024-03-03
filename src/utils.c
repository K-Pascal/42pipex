/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:08:55 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/05 16:20:59 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

void	exec_prog(char const *pathname, char *const argv[], char *const envp[])
{
	execve(pathname, argv, envp);
	perror(pathname);
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
