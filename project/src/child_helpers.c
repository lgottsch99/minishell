/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 15:21:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 13:00:52 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//closing fds in child process
void	close_child_fds(t_pipeline *pipeline, int i)
{
	if (i == 0) //first cmd
		close(pipeline->fd_pipe[i][1]);
	else if (i == pipeline->nr_cmd - 1) //last cmd
		close(pipeline->fd_pipe[i - 1][0]);
	else //inbetween
	{
		close(pipeline->fd_pipe[i - 1][0]);
		close(pipeline->fd_pipe[i][1]);
	}
}

static void	close_first(t_pipeline *pipeline, int y, int i)
{
	while (y < pipeline->nr_cmd - 1)
	{
		if (y != i)
			close(pipeline->fd_pipe[y][1]);
		close(pipeline->fd_pipe[y][0]);
		y++;
	}
}

static void	close_last(t_pipeline *pipeline, int y, int i)
{
	while (y < pipeline->nr_cmd - 1)
	{
		close(pipeline->fd_pipe[y][1]);
		if (y != i - 1)
			close(pipeline->fd_pipe[y][0]);
		y++;
	}
}

void	close_not_needed_pipe(int i, t_pipeline *pipeline)
{
	int	y;
	
	y = 0;
	//looop thru fd structure
	if (i == 0) //first cmd
		close_first(pipeline, y, i);
	else if (i == pipeline->nr_cmd - 1) //last cmd
		close_last(pipeline, y, i);
	else //inbetween
	{
		//close all except [i - 1][0]  AND [i][1]
		while (y < pipeline->nr_cmd - 1)
		{
			if (y != i)
				close(pipeline->fd_pipe[y][1]);
			if (y != i - 1)
				close(pipeline->fd_pipe[y][0]);
			y++;
		}
	}

}

void	child_redirection(t_command *tmp, int i, t_pipeline *pipeline, t_env *envp)
{
	int	in; 
	int	out;

	in = 0;
	out = 0;
	if (tmp->heredoc_file)
	{
		redirect_heredoc(pipeline, i, tmp, envp);
		in = 1;
		printf("heredoc redirected\n");
	}
	//check in/out file
	else if (tmp->input_file && in == 0) //no reading from previous pipe
	{
		red_pipeline_in(pipeline, i, tmp, envp);
		in = 1;
		printf("infile redirected\n");
	}
	if (tmp->output_file) //we will not write to pipe
	{
		red_pipeline_out(pipeline, i, tmp, envp);
		out = 1;
	}
	if (in == 0 && i > 0) //redirect in to be read out end of prev pipe
	{
		red_pipeline_pipe_in(pipeline, i , envp);
		printf("input is prev pipe\n");
	}
	if (out == 0 && i < (pipeline->nr_cmd - 1))//redirect out to be write in end of current pipe
	{
		printf("output is pipe\n");
		red_pipeline_pipe_out(pipeline, i, envp);
	}
}
