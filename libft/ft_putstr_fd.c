/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:34:08 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/11/12 18:15:36 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stddef.h>

#include "libft.h"

void	ft_putstr_fd(char s[], int fd)
{
	size_t	len;

	len = ft_strlen(s);
	write(fd, s, len);
}
