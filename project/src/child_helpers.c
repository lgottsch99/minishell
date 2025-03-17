/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 15:21:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 14:37:30 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_child_fds(t_pipeline *pipeline, int i)
{
	if (i == 0)
		close(pipeline->fd_pipe[i][1]);
	else if (i == pipeline->nr_cmd - 1)
		close(pipeline->fd_pipe[i - 1][0]);
	else
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
	if (i == 0)
		close_first(pipeline, y, i);
	else if (i == pipeline->nr_cmd - 1)
		close_last(pipeline, y, i);
	else
	{
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

void	child_redirection(t_command *tmp, int i, t_pipeline *pipeline,
	t_env *envp)
{
	int	in;
	int	out;

	in = 0;
	out = 0;
	if (tmp->heredoc_file)
	{
		redirect_heredoc(pipeline, i, tmp, envp);
		in = 1;
	}
	else if (tmp->input_file && in == 0)
	{
		red_pipeline_in(pipeline, i, tmp, envp);
		in = 1;
	}
	if (tmp->output_file)
	{
		red_pipeline_out(pipeline, i, tmp, envp);
		out = 1;
	}
	if (in == 0 && i > 0)
		red_pipeline_pipe_in(pipeline, i, envp);
	if (out == 0 && i < (pipeline->nr_cmd - 1))
		red_pipeline_pipe_out(pipeline, i, envp);
}
