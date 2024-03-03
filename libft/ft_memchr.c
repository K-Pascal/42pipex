/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:42:03 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 17:24:34 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*addr;
	unsigned char		byte;

	addr = (const unsigned char *)s;
	byte = (unsigned char)c;
	while (n--)
	{
		if (*addr == byte)
			return ((void *)(addr));
		addr++;
	}
	return (NULL);
}
