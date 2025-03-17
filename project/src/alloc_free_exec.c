/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:13:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 19:29:22 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	**alloc_fd(int nr_cmd)
{
	int	**fd_pipe;
	int	i;

	fd_pipe = (int **)malloc((nr_cmd - 1) * sizeof(int *));
	if (!fd_pipe)
	{
		perror("malloc: ");
		return (NULL);
	}
	i = 0;
	while (i < nr_cmd - 1)
	{
		fd_pipe[i] = (int *)malloc(sizeof(int) * 2);
		if (!fd_pipe[i])
		{
			free_2d_array(fd_pipe, i - 1);
			perror("malloc: ");
			return (NULL);
		}
		i++;
	}
	return (fd_pipe);
}

int	*alloc_pid(int nr_cmd)
{
	int	*pid;

	pid = (int *)malloc(sizeof(int) * nr_cmd);
	if (!pid)
	{
		perror("malloc: ");
		return (NULL);
	}
	return (pid);
}

void	free_everything_pipeline_exit(t_env *envp,
	t_pipeline *pipeline, int stat)
{
	if (envp)
		free_env_list(&envp);
	if (pipeline->fd_pipe)
		free_pipe_array(pipeline->fd_pipe, pipeline->nr_cmd);
	if (pipeline->pid)
		free(pipeline->pid);
	if (pipeline->env_array)
		free_2d_char(pipeline->env_array);
	if (pipeline->cmd_list)
		free_cmd_list(&pipeline->cmd_list);
	rl_clear_history();
	envp = NULL;
	pipeline = NULL;
	exit(stat);
}

void	free_cmd_list(t_command **cmd_list)
{
	t_command	*tmp;

	while (*cmd_list)
	{
		tmp = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		if (tmp->args)
			free_2d_char(tmp->args);
		remove_heredoc(tmp->heredoc_file);//??ok
		if (tmp->heredoc_delimetr)
			free(tmp->heredoc_delimetr);
		if (tmp->input_file)
			free(tmp->input_file);
		if (tmp->output_file)
			free(tmp->output_file);
		if (tmp->exec_path)
			free(tmp->exec_path);
		free(tmp);
		tmp = NULL;
	}
	*cmd_list = NULL;
}

void	remove_heredoc(char *heredoc_file)
{
	if (heredoc_file)
	{
		unlink(heredoc_file);
		free(heredoc_file);
		heredoc_file = NULL;
	}
}