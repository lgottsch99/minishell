/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 12:12:58 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/25 17:01:25 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	red_infile(char	*input_file)
{
	int	infile_fd;

	infile_fd = open(input_file, O_RDONLY);
	if (infile_fd == -1)
	{
		perror("Error open infile\n");
		return (1);
	}
	if (redirect(infile_fd, STDIN_FILENO) == 1)
		return (1);
	close(infile_fd);
	return (0);
}

static int	red_out_append(char *output_file)
{
	int	outfile_fd;

	outfile_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND,
			S_IRUSR | S_IWUSR);
	if (outfile_fd == -1)
	{
		perror("error open outfile\n");
		return (-1);
	}
	return (outfile_fd);
}

int	red_outfile(char *output_file, t_command *cmd)
{
	int	outfile_fd;

	if (cmd->append_mode == 0)
	{
		outfile_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR);
		if (outfile_fd == -1)
		{
			perror("error open outfile\n");
			return (1);
		}
	}
	if (cmd->append_mode == 1)
	{
		outfile_fd = red_out_append(output_file);
		if (outfile_fd == -1)
			return (1);
	}
	if (redirect(outfile_fd, STDOUT_FILENO) == 1)
		return (1);
	close(outfile_fd);
	return (0);
}

int	redirect(int fd, int fd_to_replace)
{
	if ((dup2(fd, fd_to_replace)) == -1)
	{
		perror("error dup2\n");
		return (1);
	}
	return (0);
}
