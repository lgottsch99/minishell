#include "../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;
	int		x;

	i = 0;
	while (s1[i] && s2[i])
	{
		if ((s1[i] != s2[i]) || !s1[i] || !s2[i])
		{
			x = ((unsigned char)s1[i] - (unsigned char)s2[i]);
			return (x);
		}
		i++;
	}
	return (0);
}

char	*read_heredoc(char *delimetr, int count) //TODO signals +freeing
{
	char	*line;
	char	*heredoc_filename;
	//char *temp;
	char	*str_nr;
	int		fd;


	//create filename
	str_nr = ft_itoa(count); //MALLOC
	heredoc_filename = ft_strjoin("heredoc", str_nr); //MALLOC
	free(str_nr);
	printf("\n\nheredoc filename: %s\n", heredoc_filename);

	//open tmp here doc file 
	fd = open(heredoc_filename, O_RDWR | O_CREAT | O_EXCL, 0600); // O_EXCL: if file with same name exists error, 0600 access rights: only owner can access
	if (fd == -1)
	{
		free(heredoc_filename);
		perror("Failed to create temporary file");
		return NULL;
	}
	printf("opened file, del: %s\n", delimetr);

	//ask input + write to file
	while(1)
	{
		line = readline("> ");
        if (!line)
            break;
      	// If the line matches the delimiter, stop reading
	  	if (ft_strcmp(line, delimetr) == 0 && (ft_strlen(line) == ft_strlen(delimetr)))
	  	{
			free(line);
			break;
	  	}

		// Write the line to the temporary file
		write(fd, line, strlen(line));
		write(fd, "\n", 1); // Add newline after each line
	
		free(line);	// 	temp = heredoc_content;
	}
	return (heredoc_filename); //return fd to file to store + later redirect
}

//OG OLD 
// char	*read_heredoc(char *delimetr, int count)
// {
// 	char *line;
// 	char *heredoc_content;
// 	char *temp;

// 	heredoc_content = NULL;
// 	while(1)
// 	{
// 		line = readline("> ");
// 		if(!line)
// 			break;
// 		if (ft_strcmp(line, delimetr) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		temp = heredoc_content;
// 		heredoc_content = ft_strjoin(heredoc_content, line);
// 		free(line);
// 		free(temp);
// 		temp = heredoc_content;
// 		heredoc_content = ft_strjoin(heredoc_content, "\n");
// 		free(temp);
// 	}
// 	return (heredoc_content);
// }

void	clean_heredoc(t_command *cmd)
{
	// if (cmd->heredoc_input)
	// {
	// 	free(cmd->heredoc_input);
	// 	cmd->heredoc_input = NULL;
	// }
	if (cmd->heredoc_delimetr)
	{
		free(cmd->heredoc_delimetr);
		cmd->heredoc_delimetr = NULL;
	}
}
