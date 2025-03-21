/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:12 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/20 20:16:43 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute(t_env *envp, int *exit_stat, t_command *cmd_list)
{
	int	nr_cmd;

	nr_cmd = get_nr_cmd(cmd_list);
	if (check_access(cmd_list, nr_cmd, envp, exit_stat) != 0)
	{
		printf("access error\n");
		return ;
	}
	if (nr_cmd == 1 && cmd_list->is_builtin == 1)
		only_builtin(cmd_list, envp, exit_stat);
	else
		pipeline(cmd_list, envp, exit_stat);
	g_signal_status = 0;
	return ;
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
			printf("Child PID: %d\n", getpid());
			fflush(stdout);
		
			//signal(SIGINT, handle_sigint);  // Ensure child installs the handler
			signal(SIGINT, SIG_DFL);        // OR, explicitly reset to default if needed
		
			child_process(pipeline, envp, i, tmp);  // Replace with actual execution function
		}	
		//child_process(pipeline, envp, i, tmp);
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
	int	nr_cmd;

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
