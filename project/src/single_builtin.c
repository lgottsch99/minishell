/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:04:16 by lgottsch          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/03/17 16:17:31 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

=======
/*   Updated: 2025/03/08 19:25:30 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*/

>>>>>>> dana
#include "../includes/minishell.h"

static int	init_io(t_single_red *io)
{
<<<<<<< HEAD
	io->og_in = dup(STDIN_FILENO);
	if (io->og_in == -1)
		return (-1);
	io->og_out = dup(STDOUT_FILENO);
	if (io->og_out == -1)
	{
		close(io->og_in);
		return (-1);
	}
	io->red_in = 0;
	io->red_out = 0;
	return (0);
}

static int	red_only_b_in(t_command *cmd_list, int *exit_stat, t_single_red *io)
{
=======
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
>>>>>>> dana
	if (cmd_list->heredoc_file)
	{
		if (red_infile(cmd_list->heredoc_file) == 1)
		{
			perror("redirection error: ");
<<<<<<< HEAD
			close (io->og_in);
			close (io->og_out);
			*exit_stat = 1;
			return (1);
		}
		io->red_in = 1;
=======
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
>>>>>>> dana
	}
	else if (cmd_list->input_file && io->red_in == 0)
	{
		if (red_infile(cmd_list->input_file) == 1)
		{
			perror("redirection error: ");
			close (io->og_in);
			close (io->og_out);
			*exit_stat = 1;
			return (1);
		}
		io->red_in = 1;
	}
	return (0);
}

static void	red_only_builtin(t_command *cmd_list, int *exit_stat,
	t_single_red *io)
{
	if (red_only_b_in(cmd_list, exit_stat, io) == 1)
		return ;
	if (cmd_list->output_file)
	{
		if (red_outfile(cmd_list->output_file, cmd_list) == 1)
		{
			perror("redirection error: ");
<<<<<<< HEAD
			close (io->og_in);
			close (io->og_out);
			*exit_stat = 1;
			return ;
		}
		io->red_out = 1;
	}
	if (ft_strncmp(cmd_list->args[0], "exit",
			ft_strlen(cmd_list->args[0])) == 0)
	{
		close (io->og_in);
		close (io->og_out);
	}
}

static void	restore_og_fd(t_single_red *io, int *exit_stat)
{
	if (io->red_in == 1)
	{
		if (redirect(io->og_in, STDIN_FILENO) == 1)
		{
			perror("redirection error: ");
			close (io->og_in);
			close (io->og_out);
			*exit_stat = 1;
			return ;
		}
	}
	if (io->red_out == 1)
	{
		if (redirect(io->og_out, STDOUT_FILENO) == 1)
		{
			perror("redirection error: ");
			close (io->og_in);
			close (io->og_out);
			*exit_stat = 1;
			return ;
		}
	}
}

//no need to fork + pipe
void	only_builtin(t_command *cmd_list, t_env *envp, int *exit_stat)
{
	t_single_red	io;

	if (init_io(&io) != 0)
	{
		perror("duplicating fd error: ");
		*exit_stat = 1;
		return ;
	}
	red_only_builtin(cmd_list, exit_stat, &io);
	run_builtin(cmd_list, envp, NULL, exit_stat);
	restore_og_fd(&io, exit_stat);
	remove_heredoc(cmd_list->heredoc_file);
	close (io.og_in);
	close (io.og_out);
=======
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
>>>>>>> dana
}
