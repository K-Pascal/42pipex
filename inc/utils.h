/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:11:04 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/08 17:38:05 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

typedef struct s_data
{
	char	*limiter;
	char	*f_in;
	char	*f_out;
	int		nbr_cmds;
	char	**cmds;
	char	**envp;
}	t_data;

void	exec_prog(char const *pathname, char *const argv[], char *const envp[]);
void	free_pipex(t_data *data, char **path, int fd[2], char msg[]);
void	my_free_all(char **arr);
void	my_n_free_all(char **arr, int len);
#endif
