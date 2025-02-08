/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 12:12:58 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/03 16:26:44 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	red_infile(char	*input_file)
{
	int infile_fd;

	if ((infile_fd = open(input_file, O_RDONLY)) == -1) //open infile
	{
		perror("error open infile\n");
		return; //???
	}
	redirect(infile_fd, STDIN_FILENO);
	close(infile_fd);
}


void	red_outfile(char *output_file, t_command *cmd)
{
	int outfile_fd;

	//open outfile or create (>), truncate
	if (cmd->append_mode == 0)
	{
		if ((outfile_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		{
			perror("error open outfile\n");
			return;
		}
	}
	//open outfile or create (>>), append
	if (cmd->append_mode == 1)
	{
		if ((outfile_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) == -1)
		{
			perror("error open outfile\n");
			return;
		}
	}
	redirect(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
}

void	redirect(int fd, int fd_to_replace)
{
	if((dup2(fd, fd_to_replace)) == -1)
	{
		perror("error dup2\n");
		return;
	}
	return;
}

