/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:41:37 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 17:20:33 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	my_isspace(char c)
{
	return ((c >= '\t' && c <= '\r') || c == ' ');
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	nb;

	sign = 1;
	nb = 0;
	while (my_isspace(*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
		nb = nb * 10 + *(nptr++) - '0';
	return (sign * nb);
}
