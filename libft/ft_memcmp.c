/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:01:00 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 17:32:38 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*addr1;
	unsigned char	*addr2;

	if (n == 0)
		return (0);
	addr1 = (unsigned char *)s1;
	addr2 = (unsigned char *)s2;
	while (--n)
	{
		if (*addr1 != *addr2)
			break ;
		addr1++;
		addr2++;
	}
	return (*addr1 - *addr2);
}
