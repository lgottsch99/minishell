/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:12 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 13:55:08 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute(t_env *envp, int *exit_stat, t_command *cmd_list)
{
	int	nr_cmd;

	nr_cmd = get_nr_cmd(cmd_list);
	if (nr_cmd == 1 && cmd_list->is_builtin == 1)
		only_builtin(cmd_list, envp, exit_stat);
	else
		pipeline(cmd_list, envp, exit_stat);
	g_signal_status = 0;
	return ;
}

static int	check_access_and_child(t_command *tmp, t_env *envp,
	t_pipeline *pipeline, int i)
{
	signal(SIGINT, SIG_DFL);
	if (loop_check_access(tmp, envp, pipeline) != 0)
	{
		printf("access error\n");
		return (1);
	}
	child_process(pipeline, envp, i, tmp);
	return (0);
}

static void	pipeline_loop(t_pipeline *pipeline, t_env *envp)
{
	int			i;
	t_command	*tmp;

	i = 0;
	tmp = pipeline->cmd_list;
	while (i < pipeline->nr_cmd)
	{
		pipeline->pid[i] = fork();
		if (pipeline->pid[i] < 0)
		{
			perror("fork error: ");
			free_everything_pipeline_exit(envp, pipeline, 1);
		}
		if (pipeline->pid[i] == 0)
		{
			if (check_access_and_child(tmp, envp, pipeline, i) == 1)
				free_everything_pipeline_exit(envp, pipeline,
					*pipeline->exit_stat);
		}
		else
		{
			tmp = tmp->next;
			i++;
		}
	}
}

static void	free_everything_malloced_pipe(t_pipeline *pipeline)
{
	if (pipeline->fd_pipe)
		free_pipe_array(pipeline->fd_pipe, pipeline->nr_cmd);
	if (pipeline->pid)
		free(pipeline->pid);
}

void	pipeline(t_command *cmd_list, t_env *envp, int *exit_stat)
{
	t_pipeline	pipeline;
	int			nr_cmd;

	nr_cmd = get_nr_cmd(cmd_list);
	init_pipeline(&pipeline, nr_cmd, cmd_list, exit_stat);
	create_pipes(&pipeline, envp);
	pipeline_loop(&pipeline, envp);
	close_parent_fds(&pipeline);
	signal(SIGINT, SIG_IGN);
	wait_children(&pipeline, envp);
	signal(SIGINT, handle_sigint);
	free_everything_malloced_pipe(&pipeline);
	return ;
}
