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

char	*read_heredoc(char *delimetr)
{
	char *line;
	char *heredoc_content;
	char *temp;

	heredoc_content = NULL;
	while(1)
	{
		line = readline("> ");
		if(!line)
			break;
		if (ft_strcmp(line, delimetr) == 0)
		{
			free(line);
			break;
		}
		temp = heredoc_content;
		heredoc_content = ft_strjoin(heredoc_content, line);
		free(line);
		free(temp);
		temp = heredoc_content;
		heredoc_content = ft_strjoin(heredoc_content, "\n");
		free(temp);
	}
	return (heredoc_content);
}

void	clean_heredoc(t_command *cmd)
{
	if (cmd->heredoc_input)
	{
		free(cmd->heredoc_input);
		cmd->heredoc_input = NULL;
	}
	if (cmd->heredoc_delimetr)
	{
		free(cmd->heredoc_delimetr);
		cmd->heredoc_delimetr = NULL;
	}
}
