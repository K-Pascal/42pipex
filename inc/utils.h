/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:11:04 by pnguyen-          #+#    #+#             */
/*   Updated: 2024/03/09 19:53:35 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int		redirect_pipefd(int fd, int newfd);
void	close_pipe(int fds[2]);
void	my_perror(char const name[], char const msg[]);

#endif
