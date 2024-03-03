/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnguyen- <pnguyen-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:36:32 by pnguyen-          #+#    #+#             */
/*   Updated: 2023/11/24 15:52:19 by pnguyen-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "get_next_line.h"

static char	*absorb_buffer(char *buffer, int size, int *len);
static char	*my_alloccpy(t_line *line, int size);
static void	my_realloc(t_line *line, int targetsize);
static int	append_line(t_line *line, char buffer[], int bytes, int i);

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	ssize_t		bytes;
	t_line		line;
	int			i;

	if (fd < 0 || BUFFER_SIZE == 0)
		return (NULL);
	line.content = absorb_buffer(buffer, BUFFER_SIZE + 1, &line.len);
	if (line.content != NULL && buffer[0] != '\0')
		return (line.content);
	line.size = line.len + 1;
	bytes = read(fd, buffer, BUFFER_SIZE);
	while (bytes > 0)
	{
		buffer[bytes] = '\0';
		i = isendl(buffer, bytes);
		if (append_line(&line, buffer, bytes, i) <= 0)
			break ;
		bytes = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes <= 0 && line.len == 0)
		return (NULL);
	line.content = my_alloccpy(&line, line.len + 1);
	return (line.content);
}

static char	*absorb_buffer(char *buffer, int size, int *len)
{
	char	*line;
	int		pos;
	int		isnl;

	*len = 0;
	if (buffer[0] == '\0')
		return (NULL);
	pos = isendl(buffer, size);
	isnl = buffer[pos] == '\n';
	line = malloc((pos + 1 + isnl) * sizeof(char));
	if (line == NULL)
		return (NULL);
	if (pos != -1)
	{
		*len = pos + isnl;
		my_strlcpy(line, buffer, *len + 1);
		my_strlcpy(buffer, buffer + pos + isnl, size - pos);
	}
	else
	{
		*len = my_strlen(buffer);
		my_strlcpy(line, buffer, *len + 1);
		buffer[0] = '\0';
	}
	return (line);
}

static int	append_line(t_line *line, char buffer[], int bytes, int i)
{
	int	isnl;

	i = isendl(buffer, bytes);
	if (i == -1)
	{
		my_realloc(line, line->len + bytes + 1);
		if (line->content == NULL)
			return (-1);
		line->len += bytes;
		my_strlcat(line->content, buffer, line->len - bytes, line->len + 1);
		buffer[0] = '\0';
		return (1);
	}
	else
	{
		isnl = buffer[i] == '\n';
		my_realloc(line, line->len + i + isnl + 1);
		if (line->content == NULL)
			return (-1);
		line->len += i + isnl;
		my_strlcat(line->content, buffer, line->len - i - isnl, line->len + 1);
		my_strlcpy(buffer, buffer + i + 1, BUFFER_SIZE - i);
		buffer[BUFFER_SIZE - i] = '\0';
		return (0);
	}
}

static void	my_realloc(t_line *line, int targetsize)
{
	while (line->size < targetsize)
	{
		line->size *= 2;
		if (line->size < targetsize)
			continue ;
		line->content = my_alloccpy(line, line->size);
	}
}

static char	*my_alloccpy(t_line *line, int size)
{
	char	*temp;

	temp = malloc(size * sizeof(char));
	if (temp == NULL)
	{
		free(line->content);
		return (NULL);
	}
	my_strlcpy(temp, line->content, line->len + 1);
	free(line->content);
	line->content = temp;
	return (line->content);
}
