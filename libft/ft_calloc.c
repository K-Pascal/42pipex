/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:58:35 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 16:33:43 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*array;

	if (size != 0 && nmemb > (size_t)(-1) / size)
		return (NULL);
	array = malloc(nmemb * size);
	if (array != NULL)
		ft_bzero(array, nmemb * size);
	return (array);
}
