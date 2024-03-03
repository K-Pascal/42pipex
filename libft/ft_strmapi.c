/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 19:05:00 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 18:36:18 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "libft.h"

char	*ft_strmapi(char const s[], char (*f)(unsigned int, char))
{
	char	*dest;
	size_t	i;
	size_t	len;

	len = ft_strlen(s);
	dest = ft_calloc(len + 1, sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		*dest = f(i++, *(s++));
		dest++;
	}
	return (dest - i);
}
