/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:04:16 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/08 19:25:30 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*/

#include "../includes/minishell.h"

int	run_builtin(t_command *cmd_list, t_env *envp, t_pipeline *pipeline)
{
	int exit_stat;

	exit_stat = 0;
	printf("choosing builtin ft\n");
	
	if (ft_strncmp(cmd_list->args[0], "env", ft_strlen(cmd_list->args[0])) == 0)
		exit_stat = print_env(envp);
	else if (ft_strncmp(cmd_list->args[0], "echo", ft_strlen(cmd_list->args[0])) == 0)
		exit_stat = echo(cmd_list);
	else if (ft_strncmp(cmd_list->args[0], "pwd", ft_strlen(cmd_list->args[0])) == 0)
		exit_stat = pwd();
	else if (ft_strncmp(cmd_list->args[0], "exit", ft_strlen(cmd_list->args[0])) == 0)
	 	exit_stat = exit_shell(cmd_list, envp, pipeline);
	else if (ft_strncmp(cmd_list->args[0], "cd", ft_strlen(cmd_list->args[0])) == 0)
		exit_stat = cd(cmd_list, envp);
	else if (ft_strncmp(cmd_list->args[0], "export", ft_strlen(cmd_list->args[0])) == 0)
		exit_stat = eexport(cmd_list, envp);
	else if (ft_strncmp(cmd_list->args[0], "unset", ft_strlen(cmd_list->args[0])) == 0)
		exit_stat = unset(cmd_list, envp);
	return (exit_stat);
}


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
	return (exit_stat);
}
