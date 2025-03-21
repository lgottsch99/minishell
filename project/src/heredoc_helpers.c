/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:08:08 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/21 20:21:18 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredoc_input(int fd, char *delimetr)
{
	char	*line;

	//set custom sig handler 
	struct sigaction sa_new;
	struct sigaction old_sa;

	sa_new.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, &old_sa);

	g_signal_status = 0; // Reset before entering the loop

	while (1)
	{
		line = readline("> ");
		// Check if Ctrl+C was pressed
		if (!line || g_signal_status == SIGINT)
		{
			close(fd);
			sigaction(SIGINT, &old_sa, NULL);
			open("/dev/tty", O_RDONLY); 
			return (1);
		}
		if (ft_strcmp(line, delimetr) == 0
			&& (ft_strlen(line) == ft_strlen(delimetr)))
		{
			free(line);
			close (fd);
			break ;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	sigaction(SIGINT, &old_sa, NULL); // Restore original SIGINT handler
	g_signal_status = 0; // Reset before entering the loop

	return (0);
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
	if (heredoc_input(fd, delimetr) != 0)
	{
		unlink(heredoc_filename);
		free(heredoc_filename);
		return (NULL);
	}
	return (heredoc_filename);
}
