/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiylee <jiylee@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 11:20:09 by jiylee            #+#    #+#             */
/*   Updated: 2026/02/27 20:28:23 by jiylee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static ssize_t	read_and_store(int fd, char **backup, char *buf)
{
	char		*new_str;
	ssize_t		bytes_read;

	bytes_read = 1;
	while (1)
	{
		if (ft_strchr(*backup, '\n'))
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buf[bytes_read] = '\0';
		new_str = ft_strjoin(*backup, buf);
		free(*backup);
		*backup = new_str;
	}
	return (bytes_read);
}

static char	*extract_line(char **backup)
{
	char			*newline_position;
	char			*result;
	char			*initial;
	char			*after_newline;
	unsigned int	index;

	initial = *backup;
	newline_position = ft_strchr(*backup, '\n');
	if (newline_position)
	{
		result = ft_substr(*backup, 0, newline_position - *backup + 1);
		index = newline_position - *backup + 1;
		after_newline = ft_substr(*backup, index, ft_strlen(*backup) - index);
		free(initial);
		*backup = after_newline;
		return (result);
	}
	else
	{
		result = ft_substr(*backup, 0, ft_strlen(*backup));
		*backup = malloc(1);
		free(initial);
		**backup = '\0';
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*backup;
	char		*buf;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	if (buf == NULL)
		return (NULL);
	if (backup == NULL)
	{
		backup = malloc(1);
		if (backup == NULL)
			return (NULL);
		*backup = '\0';
	}
	bytes_read = read_and_store(fd, &backup, buf);
	free(buf);
	if (bytes_read == -1 || (bytes_read == 0 && *backup == '\0'))
	{
		free(backup);
		backup = NULL;
		return (NULL);
	}
	return (extract_line(&backup));
}
