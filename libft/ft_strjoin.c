/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:39:17 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 18:30:40 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const s1[], char const s2[])
{
	char	*dest;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	dest = ft_calloc(len1 + len2 + 1, sizeof(char));
	if (dest == 0)
		return (0);
	ft_strlcpy(dest, s1, len1 + 1);
	ft_strlcpy(dest + len1, s2, len1 + len2 + 1);
	return (dest);
}
