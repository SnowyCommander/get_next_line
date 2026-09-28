/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiylee <jiylee@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 11:20:20 by jiylee            #+#    #+#             */
/*   Updated: 2026/02/27 19:39:00 by jiylee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	const char	*p = str;

	while (*p++ != '\0')
	{
	}
	return (p - str - 1);
}

char	*ft_strchr(char *str, char c)
{
	if (str == NULL)
	{
		return (NULL);
	}
	while (*str != c && *str != '\0')
	{
		++str;
	}
	if (*str == '\0')
	{
		return (NULL);
	}
	else
	{
		return (str);
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total;
	char	*s1_ptr;
	char	*start;

	if (s1 == NULL || s2 == NULL)
	{
		return (NULL);
	}
	total = ft_strlen(s1) + ft_strlen(s2) + 1;
	s1_ptr = malloc(total);
	if (s1_ptr == NULL)
	{
		return (NULL);
	}
	start = s1_ptr;
	while (*s1)
		*s1_ptr++ = *s1++;
	while (*s2)
		*s1_ptr++ = *s2++;
	*s1_ptr = '\0';
	return (start);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	char	*origin;
	char	*src;
	size_t	slen;

	if (s == NULL)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
		len = 0;
	if (slen - start < len)
		len = slen - start;
	ptr = malloc(len + 1);
	if (ptr == NULL)
		return (NULL);
	origin = ptr;
	src = (char *)(s + start);
	while (len-- > 0 && *src)
	{
		*ptr++ = *src++;
	}
	*ptr = '\0';
	return (origin);
}
