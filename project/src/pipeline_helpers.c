/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:43:25 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/09 15:21:59 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int get_nr_cmd(t_command *cmd_list)
{
	int			nr;
	t_command 	*tmp;

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

void	init_pipeline(t_pipeline *pipeline, int nr_cmd, t_command *cmd_list, int *exit_stat)
{
	if (nr_cmd > 1)
		pipeline->fd_pipe = alloc_fd(nr_cmd); //MALLOC
	else
		pipeline->fd_pipe = NULL;
	//if (nr_cmd > 1)
	pipeline->pid = alloc_pid(nr_cmd); //MALLOC
	//else
	//	pipeline->pid = NULL;
	pipeline->env_array = NULL;
	pipeline->cmd_list = cmd_list;
	pipeline->nr_cmd = nr_cmd;
	pipeline->exit_stat = exit_stat;
	// if (!pipeline->fd_pipe || !pipeline->pid) //check if ok with only one cmd TODO
	// {
	// 	printf("Error allocating memory. Exiting minishell\n");
	// 	free_everything_pipeline_exit(envp, pipeline, 1, nr_cmd);
	// } 
}

void	create_pipes(t_pipeline *pipeline, t_env *envp)
{
	int i;
	
	i = 0;
	while (i < pipeline->nr_cmd - 1)
	{
		printf("creating piep\n");
		if (pipe(pipeline->fd_pipe[i]) == -1)
		{
			perror("pipe: ");
			free_everything_pipeline_exit(envp, pipeline, 1);
			//free more?
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

void	wait_children(t_pipeline *pipeline,t_env *envp)
{
	int	y;
	//wait for all children TO DO whatif terminated by SIGNAL? WTERMSIG
	y = 0;
	while (y < pipeline->nr_cmd)
	{
		printf("waiting\n");
		//wait(exit_stat);//save exit stat
		if (waitpid(pipeline->pid[y], pipeline->exit_stat, 0) == -1)
		{
			perror("waitpid: ");
			free_everything_pipeline_exit(envp, pipeline, 1);
			//return ? or exit
		}
		//extract real exit_stat + save in env??
		if (WIFEXITED(*pipeline->exit_stat))
			*pipeline->exit_stat = WEXITSTATUS(*pipeline->exit_stat); //check + test if ok?
		y++;
	}	// make sure last process exit is stored????
}
