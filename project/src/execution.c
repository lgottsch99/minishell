/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution->c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:12 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/09 16:34:02 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pipeline(t_command *cmd_list, int nr_cmd, t_env *envp, int *exit_stat); //works for 2 -> n cmds 


void	execute(t_env *envp, int *exit_stat, t_command *cmd_list)
{
	int	nr_cmd;
	//get size of lists 
	nr_cmd = get_nr_cmd(cmd_list);
	printf("\n\nsize cmd list: %i\n\n", nr_cmd);

	print_commands(cmd_list);
	printf("\n\n");
	//check access of everything (files + cmds), creates paths, decides if builtin
	if (check_access(cmd_list, nr_cmd, envp) != 0)
	{
		printf("access error\n");
		*exit_stat = 1;
		return;
	}
	printf("access ok\n");
	
	//SPECIAL CASE nur ein cmd + builtin: dann kein fork!
	if (nr_cmd == 1 && cmd_list->is_builtin == 1)
		*exit_stat = only_builtin(cmd_list, envp);
	// set up pipes (if cmd is builtin they are forked as well, but might have no effect on the main shell p)
	else
		pipeline(cmd_list, nr_cmd, envp, exit_stat);
	return;
}


void	pipeline(t_command *cmd_list, int nr_cmd, t_env *envp, int *exit_stat) //works for 2 -> n cmds 
{
	printf("in pipeline\n");

	t_pipeline	pipeline;

	init_pipeline(&pipeline, nr_cmd, cmd_list, exit_stat); //MALLOC
	create_pipes(&pipeline, envp);//1. create ALLLL pipes necessary
	
	pipeline_loop(&pipeline, envp);

	printf("closing pipe fds\n");

	close_parent_fds(&pipeline);//close all open fildes
	wait_children(&pipeline, envp);
	free_everything_malloced_pipe(&pipeline);

	printf("waited for all ps and finished\n");
	return;
}

void	pipeline_loop(t_pipeline *pipeline, t_env *envp)
{
	int			i;
	t_command	*tmp;

	i = 0;
	tmp = pipeline->cmd_list;
	while (i < pipeline->nr_cmd) //main loop for fork, exec 
	{
		if ((pipeline->pid[i] = fork()) < 0)//fork process
		{
			perror("fork error: ");
			free_everything_pipeline_exit(envp, pipeline, 1);
		}
		if (pipeline->pid[i] == 0) //in child to exec cmd
			child_process(pipeline, envp, i, tmp);
		else // in parent 
		{
			tmp = tmp->next;
			i++;
		}
	}
}

void	free_everything_malloced_pipe(t_pipeline *pipeline)
{
	if (pipeline->fd_pipe)
		free_pipe_array(pipeline->fd_pipe, pipeline->nr_cmd);
	if (pipeline->pid)
		free(pipeline->pid);
}
