/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:14:24 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 18:34:03 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char dst[], const char src[], size_t size)
{
	size_t	len_s;

	len_s = 0;
	while (*src != '\0')
	{
		if (++len_s < size)
			*(dst++) = *src;
		src++;
	}
	if (size != 0)
		*dst = '\0';
	return (len_s);
}
