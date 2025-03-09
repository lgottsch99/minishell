/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 12:12:58 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/08 17:40:13 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	red_infile(char	*input_file)
{
	int infile_fd;

	if ((infile_fd = open(input_file, O_RDONLY)) == -1)
	{
		perror("error open infile\n");
		return 1; //TO DO check if error code?
	}
	if (redirect(infile_fd, STDIN_FILENO) == 1)
		return 1;
	close(infile_fd);
	return 0;
}


int	red_outfile(char *output_file, t_command *cmd)
{
	int outfile_fd;

	//open outfile or create (>), truncate
	if (cmd->append_mode == 0)
	{
		if ((outfile_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		{
			perror("error open outfile\n");
			return 1;
		}
	}
	//open outfile or create (>>), append
	if (cmd->append_mode == 1)
	{
		if ((outfile_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) == -1)
		{
			perror("error open outfile\n");
			return 1;
		}
	}
	if (redirect(outfile_fd, STDOUT_FILENO) == 1)
		return 1;
	close(outfile_fd);
	return 0;
}

int	redirect(int fd, int fd_to_replace)
{
	if((dup2(fd, fd_to_replace)) == -1)
	{
		perror("error dup2\n");
		return 1;
	}
	return 0;
}

