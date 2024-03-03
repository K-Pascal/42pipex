/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:54:12 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 18:52:00 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

static char	*my_strndup(char const src[], size_t n)
{
	char	*dest;

	dest = ft_calloc(n + 1, sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, src, n + 1);
	return (dest);
}

char	*ft_strtrim(char const s1[], char const set[])
{
	char		*dest;
	char const	*start;

	while (*s1 != '\0' && ft_strchr(set, *s1) != NULL)
		s1++;
	start = s1;
	while (*s1 != '\0')
		s1++;
	s1--;
	while (start < s1 && ft_strchr(set, *s1) != NULL)
		s1--;
	s1++;
	dest = my_strndup(start, s1 - start);
	return (dest);
}
