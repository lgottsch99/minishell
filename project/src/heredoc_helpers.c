/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:08:08 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 14:09:22 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	set_heredoc_signal(struct sigaction *sa_new,
	struct sigaction *old_sa)
{
	sa_new->sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_new->sa_mask);
	sa_new->sa_flags = 0;
	sigaction(SIGINT, sa_new, old_sa);
	g_signal_status = 0;
}

static void	handle_ctrlc(int fd, struct sigaction *old_sa)
{
	close(fd);
	sigaction(SIGINT, old_sa, NULL);
	open("/dev/tty", O_RDONLY);
}

static void	write_free(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

int	heredoc_input(int fd, char *delimetr)
{
	char				*line;
	struct sigaction	sa_new;
	struct sigaction	old_sa;

	set_heredoc_signal(&sa_new, &old_sa);
	while (1)
	{
		line = readline("> ");
		if (!line || g_signal_status == SIGINT)
		{
			handle_ctrlc(fd, &old_sa);
			return (1);
		}
		if (ft_strcmp(line, delimetr) == 0
			&& (ft_strlen(line) == ft_strlen(delimetr)))
		{
			free(line);
			close (fd);
			break ;
		}
		write_free(fd, line);
	}
	sigaction(SIGINT, &old_sa, NULL);
	g_signal_status = 0;
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
