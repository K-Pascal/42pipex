/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:24:18 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 18:58:24 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

#include "libft.h"

char	*ft_substr(char const s[], unsigned int start, size_t len)
{
	char	*str;
	size_t	len_s;

	if (len == 0)
		return (ft_strdup(""));
	len_s = ft_strlen(s);
	if (start + len > len_s)
	{
		if ((long)(len_s - start) < 0)
			len = 0;
		else
			len = len_s - start;
	}
	str = malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (len == 0)
		str[0] = '\0';
	else
		ft_strlcpy(str, &s[start], len + 1);
	return (str);
}
