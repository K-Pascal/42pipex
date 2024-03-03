/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:36:52 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 18:30:18 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char s[], int c)
{
	char	character;

	character = (char)c;
	while (*s != character)
	{
		if (*s == '\0')
			return (0);
		s++;
	}
	return ((char *)s);
}
