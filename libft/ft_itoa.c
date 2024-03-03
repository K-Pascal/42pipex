/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:11:18 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 17:14:23 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_digits(int n)
{
	int	len;

	len = 0;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*digits;
	int		len;

	if (n == 0)
		return (ft_strdup("0"));
	len = num_digits(n) + (n < 0);
	digits = ft_calloc(len + 1, sizeof(char));
	if (digits == 0)
		return (0);
	digits[0] = '-';
	while (n)
	{
		if (n < 0)
			digits[--len] = -(n % 10) + '0';
		else
			digits[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (digits);
}
