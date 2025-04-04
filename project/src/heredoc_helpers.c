/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:08:08 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 13:14:32 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc_input(int fd, char *delimetr)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimetr) == 0
			&& (ft_strlen(line) == ft_strlen(delimetr)))
		{
			free (line);
			close (fd);
			break ;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

char	*read_heredoc(char *delimetr, int count)
{
	char	*heredoc_filename;
	char	*str_nr;
	int		fd;

	str_nr = ft_itoa(count);
	heredoc_filename = ft_strjoin("heredoc", str_nr);
	free(str_nr);
	fd = open(heredoc_filename, O_RDWR | O_CREAT | O_EXCL, 0666);
	if (fd == -1)
	{
		free(heredoc_filename);
		perror("Failed to create temporary file");
		return (NULL);
	}
	heredoc_input(fd, delimetr);
	return (heredoc_filename);
}
