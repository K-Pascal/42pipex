/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:28:42 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/12/03 17:42:34 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

static size_t	count_words(char const s[], char c)
{
	size_t	count;

	count = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			count++;
			while (*s != c && *s != '\0')
				s++;
		}
		else
			s++;
	}
	return (count);
}

static size_t	len_word(char const s[], char c)
{
	char const	*start;

	start = s;
	while (*s != c && *s != '\0')
		s++;
	return (s - start);
}

static char	*my_strndup(char const s[], size_t n)
{
	char	*dest;

	dest = ft_calloc(n + 1, sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, s, n + 1);
	return (dest);
}

static void	my_free_all(char **arr, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_split(char const s[], char c)
{
	char	**arr_str;
	size_t	num_words;
	size_t	len;
	size_t	i;

	num_words = count_words(s, c);
	arr_str = ft_calloc(num_words + 1, sizeof(char *));
	if (arr_str == NULL)
		return (NULL);
	i = 0;
	while (i < num_words)
	{
		while (*s == c && *s != '\0')
			s++;
		len = len_word(s, c);
		arr_str[i] = my_strndup(s, len);
		if (arr_str[i++] == NULL)
		{
			my_free_all(arr_str, i);
			return (NULL);
		}
		s += len;
	}
	return (arr_str);
}
