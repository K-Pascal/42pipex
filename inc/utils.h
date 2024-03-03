/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:11:04 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/01/10 18:56:30 by pnguyen-         ###   ########.fr       */
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

void	redirect_pipefd(int fd, int newfd);
void	close_pipe(int fds[2]);
void	my_free_all(char **arr);
void	prepare_command(t_data *data, int i);
#endif
