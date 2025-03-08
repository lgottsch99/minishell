/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:08:08 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/08 19:08:11 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc_input(int fd, char *delimetr)
{
	char *line;

	while(1)
	{
		line = readline("> ");
        if (!line)
            break;
      	// If the line matches the delimiter, stop reading
	  	if (ft_strcmp(line, delimetr) == 0 && (ft_strlen(line) == ft_strlen(delimetr)))
	  	{
			free(line);
			close (fd);
			break;
	  	}

		// Write the line to the temporary file
		write(fd, line, strlen(line));
		write(fd, "\n", 1); // Add newline after each line
		free(line);	// 	temp = heredoc_content;
	}

}

char	*read_heredoc(char *delimetr, int count) //TODO signals +freeing
{
	char	*heredoc_filename;
	char	*str_nr;
	int		fd;

	//create filename
	str_nr = ft_itoa(count); //MALLOC
	heredoc_filename = ft_strjoin("heredoc", str_nr); //MALLOC
	free(str_nr);
	printf("\n\nheredoc filename: %s\n", heredoc_filename);

	//open tmp here doc file 
	fd = open(heredoc_filename, O_RDWR | O_CREAT | O_EXCL, 0666); // O_EXCL: if file with same name exists error, 0600 access rights: only owner can access
	if (fd == -1)
	{
		free(heredoc_filename);
		perror("Failed to create temporary file");
		return NULL;
	}
	printf("opened file, del: %s\n", delimetr);

	heredoc_input(fd, delimetr);
	return (heredoc_filename); //return fd to file to store + later redirect
}
