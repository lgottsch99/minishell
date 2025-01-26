/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:52:10 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/25 16:13:57 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../full_libft.h"

/*
returns:
Read line: correct behavior
NULL: there is nothing else to read, or an error
occurred
*/

//use double pointer so u can operate on static pointer
static char	*get_nl_and_update(char	**leftover)
{
	int		index;
	char	*newline;
	char	*updated;

	index = getindexnl(*leftover);
	newline = ft_substr(*leftover, 0, index + 1);
	if (!newline)
		return (NULL);
	updated = ft_substr(*leftover, index + 1, ft_strlen(*leftover));
	free(*leftover);
	*leftover = NULL;
	if (!updated)
		return (NULL);
	else
		*leftover = updated;
	return (newline);
}

static int	is_nl(char *new)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(new))
	{
		if (new[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static char	*make_new(char **leftover, char *buf)
{
	char	*new;

	if (*leftover)
	{
		new = ft_strjoin(*leftover, buf);
		free(*leftover);
		*leftover = NULL;
		if (!new)
			return (NULL);
	}
	else
	{
		new = ft_substr(buf, 0, ft_strlen(buf));
		if (!new)
			return (NULL);
	}
	return (new);
}

static char	*read_until_nl(int fd, char *buf, char **leftover)
{
	char	*new;
	int		bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(*leftover), *leftover = NULL, NULL);
		if (bytes_read == 0)
			break ;
		buf[bytes_read] = '\0';
		new = make_new(leftover, buf);
		if (!new)
			return (NULL);
		*leftover = new;
		new = NULL;
		if (is_nl(*leftover))
			break ;
	}
	return (*leftover);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*buf;
	char		*newline;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (free(leftover), leftover = NULL, NULL);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (free(buf), free(leftover), leftover = NULL, NULL);
	leftover = read_until_nl(fd, buf, &leftover);
	free(buf);
	buf = NULL;
	if (!leftover)
		return (NULL);
	newline = get_nl_and_update(&leftover);
	if (!leftover || ft_strlen(leftover) == 0)
	{
		free (leftover);
		leftover = NULL;
	}
	return (newline);
}
