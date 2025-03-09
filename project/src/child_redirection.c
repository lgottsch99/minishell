/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:13:35 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/09 16:14:04 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	redirect_heredoc(t_pipeline *pipeline, int i, t_command *tmp, t_env *envp)
{
	if (pipeline->nr_cmd > 1 && i > 0) //if not in first
		close(pipeline->fd_pipe[i - 1][0]);
		
	printf("cmd heredoc file: %s\n", tmp->heredoc_file);
	if (red_infile(tmp->heredoc_file) == 1)
	{
		perror("redirection error: ");
		free_everything_pipeline_exit(envp, pipeline, 1);
	}
	unlink(tmp->heredoc_file);
}

void	red_pipeline_in(t_pipeline *pipeline, int i, t_command *tmp, t_env *envp)
{
	if (pipeline->nr_cmd > 1 && i > 0) //if not in first
		close(pipeline->fd_pipe[i - 1][0]);
		
	if (red_infile(tmp->input_file) == 1)
	{
		perror("redirection error: ");
		free_everything_pipeline_exit(envp, pipeline, 1);
	}
}

void	red_pipeline_out(t_pipeline *pipeline, int i, t_command *tmp, t_env *envp)
{
	if (pipeline->nr_cmd > 1 && i < pipeline->nr_cmd - 1)
		close(pipeline->fd_pipe[i][1]);

	printf("outfile redirected\n");
	if (red_outfile(tmp->output_file, tmp) == 1)
	{
		perror("redirection error: ");
		free_everything_pipeline_exit(envp, pipeline, 1);
	}
}

void	red_pipeline_pipe_in(t_pipeline *pipeline, int i, t_env *envp)
{
	if (redirect(pipeline->fd_pipe[i - 1][0], STDIN_FILENO) == 1)
	{
		perror("redirection error: ");
		free_everything_pipeline_exit(envp, pipeline, 1);
	}
}

void	red_pipeline_pipe_out(t_pipeline *pipeline, int i, t_env *envp)
{
	if (redirect(pipeline->fd_pipe[i][1], STDOUT_FILENO) == 1)
	{
		perror("redirection error: ");
		free_everything_pipeline_exit(envp, pipeline, 1);
	}
}