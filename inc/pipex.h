/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:54:58 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/09 19:12:03 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_data
{
	char	*limiter;
	char	*f_in;
	char	*f_out;
	int		nbr_cmds;
	char	**cmds;
	char	**envp;
}	t_data;

void	pipex(t_data *data, int *status);

#endif
