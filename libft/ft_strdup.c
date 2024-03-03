/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:33:38 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 17:46:22 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

char	*ft_strdup(const char s[])
{
	char	*dest;
	size_t	len;

	len = ft_strlen(s);
	dest = malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, s, len + 1);
	return (dest);
}
