/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:04:16 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/09 13:55:17 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*/

#include "../includes/minishell.h"

static int	init_io(t_single_red *io)
{
	io->og_in = dup(STDIN_FILENO); //ADD PROTECT  TODO
	if (io->og_in == -1)
		return -1;
	io->og_out = dup(STDOUT_FILENO); //SAME
	if (io->og_out == -1)
	{
		close ( io->og_in);
		return -1;
	}
	io->red_in = 0;
	io->red_out = 0;
	return 0;
}

int	only_builtin(t_command *cmd_list, t_env *envp) //no need to fork + pipe
{
	printf("running single builtin\n");
	
	t_single_red	io;
	int 			exit_stat;

	init_io(&io);
	exit_stat = 0;
	
	// redirect if needed
	if (cmd_list->heredoc_file)
	{
		if (red_infile(cmd_list->heredoc_file) == 1)
		{
			perror("redirection error: ");
			close (io.og_in);
			close (io.og_out);
			return 1;
		}

		//unlink (cmd_list->heredoc_file);
		cmd_list->heredoc_file = NULL;
		io.red_in = 1;
	}
	else if (cmd_list->input_file && io.red_in == 0)
	{
		if (red_infile(cmd_list->input_file) == 1)
		{
			perror("redirection error: ");
			close (io.og_in);
			close (io.og_out);
			return 1;
			//free + return ;
		}
		io.red_in = 1;
	}
	if (cmd_list->output_file)
	{
		if (red_outfile(cmd_list->output_file, cmd_list) == 1)
		{
			perror("redirection error: ");
			close (io.og_in);
			close (io.og_out);
			return 1;
			//free + return ;
		}

		io.red_out = 1;
	}
	//if builtin is exit close fds
	if (ft_strncmp(cmd_list->args[0], "exit", ft_strlen(cmd_list->args[0])) == 0)
	{
		close (io.og_in);
		close (io.og_out);
	}
	//go to function and run
	exit_stat = run_builtin(cmd_list, envp, NULL);

	// restore og fildes
	if (io.red_in == 1)
	{

		if (redirect(io.og_in, STDIN_FILENO) == 1)
		{
			perror("redirection error: ");
			close (io.og_in);
			close (io.og_out);
			return 1;
			//free + return ;
		}
	}
	//else
	//close(io.og_in);
	if (io.red_out == 1)
	{
	
		if (redirect(io.og_out, STDOUT_FILENO) == 1)
		{
			perror("redirection error: ");
			close (io.og_in);
			close (io.og_out);
			return 1;
			//free + return ;
		}

	}
	//else
	//close(io.og_out);
	close (io.og_in);
	close (io.og_out);
	return (exit_stat);
}
