/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:24:17 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 18:52:18 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strncmp(const char s1[], const char s2[], size_t n)
{
	if (n == 0)
		return (0);
	while (--n)
	{
		if (*s1 != *s2)
			break ;
		if (*s1 == '\0')
			break ;
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
