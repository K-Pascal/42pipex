/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:24:56 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 17:30:30 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*addr;
	unsigned char	byte;

	addr = (unsigned char *)s;
	byte = (unsigned char)c;
	while (n--)
	{
		*addr = byte;
		addr++;
	}
	return (s);
}
