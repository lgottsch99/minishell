/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:43:25 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 15:29:28 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_nr_cmd(t_command *cmd_list)
{
	int			nr;
	t_command	*tmp;

	if (cmd_list)
		nr = 1;
	else
		return (0);
	tmp = cmd_list;
	while (tmp->next)
	{
		nr++;
		tmp = tmp->next;
	}
	return (nr);
}

void	init_pipeline(t_pipeline *pipeline, int nr_cmd, t_command *cmd_list,
	int *exit_stat)
{
	if (nr_cmd > 1)
		pipeline->fd_pipe = alloc_fd(nr_cmd);
	else
		pipeline->fd_pipe = NULL;
	pipeline->pid = alloc_pid(nr_cmd);
	pipeline->env_array = NULL;
	pipeline->cmd_list = cmd_list;
	pipeline->nr_cmd = nr_cmd;
	pipeline->exit_stat = exit_stat;
}

void	create_pipes(t_pipeline *pipeline, t_env *envp)
{
	int	i;

	i = 0;
	while (i < pipeline->nr_cmd - 1)
	{
		if (pipe(pipeline->fd_pipe[i]) == -1)
		{
			perror("pipe: ");
			free_everything_pipeline_exit(envp, pipeline, 1);
		}
		i++;
	}
}

void	close_parent_fds(t_pipeline *pipeline)
{
	int	y;

	y = 0;
	while (y < pipeline->nr_cmd - 1)
	{
		close(pipeline->fd_pipe[y][1]);
		close(pipeline->fd_pipe[y][0]);
		y++;
	}
}

//wait for all children TO DO whatif terminated by SIGNAL? WTERMSIG
void	wait_children(t_pipeline *pipeline, t_env *envp)
{
	int	y;

	y = 0;
	while (y < pipeline->nr_cmd)
	{
		if (waitpid(pipeline->pid[y], pipeline->exit_stat, 0) == -1)
		{
			perror("waitpid: ");
			free_everything_pipeline_exit(envp, pipeline, 1);
		}
		if (WIFEXITED(*pipeline->exit_stat))
			*pipeline->exit_stat = WEXITSTATUS(*pipeline->exit_stat);
		y++;
	}
}
