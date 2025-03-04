/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:13:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/27 14:09:48 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_2d_array(int **fd_pipe, int size)
{
	while (size > 0)
	{
		free(fd_pipe[size]);
		size--;
	}
	free(fd_pipe);
	fd_pipe = NULL;
}


int **alloc_fd(int nr_cmd, t_env *envp)
{
	int **fd_pipe;
	int	i;

	i = 0;
	//malloc space for nr of pointers to pipes
	fd_pipe = (int **)malloc((nr_cmd - 1) * sizeof(int *));
	if (!fd_pipe)
	{
		free_env_list(&envp);
		//free more?
		perror("malloc: ");
		exit (35498);
	}
	//in loop maloc space for each pipe
	while (i < nr_cmd - 1)
	{
		fd_pipe[i] = (int *)malloc(sizeof(int) * 2);
		if (!fd_pipe[i])
		{
			//free 2d array before 
			free_2d_array(fd_pipe, i);
			free_env_list(&envp);
			//free everything else 
			perror("malloc: ");
			exit(777);
		}
		i++;
	}
	return (fd_pipe);
}

int *alloc_pid(int nr_cmd, t_env *envp)
{
	int *pid;

	pid = (int *)malloc(sizeof(int) * nr_cmd);
	if (!pid)
	{
		free_env_list(&envp);
		//free everything
		perror("malloc: ");
		exit(348); //TODO
	}
	return (pid);
}

void	free_env_list(t_env **env)
{
	t_env *tmp;

	while (*env)
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->key);
		tmp->key = NULL;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
	}
	*env = NULL;
}

void	free_pipe_array(int **fd_pipe)
{
	int	i;

	i = 0;
	while (fd_pipe[i])
	{
		free(fd_pipe[i]);
		i++;
	}
	free(fd_pipe);
	fd_pipe = NULL;
}

void	free_everything_pipeline_exit(t_env *envp, t_pipeline *pipeline)	//t_env *envp, int **fd_pipe, int *pid)
{
	if (envp)
		free_env_list(&envp);
	if (pipeline->fd_pipe)
		free_pipe_array(pipeline->fd_pipe);
	if (pipeline->pid)
		free(pipeline->pid);
	if (pipeline->env_array)
		free_2d_char(pipeline->env_array);
	//free cmd_list if not statically alloced
	if (pipeline->cmd_list)
		free_cmd_list(&pipeline->cmd_list);
	envp = NULL;
	pipeline = NULL;
	exit(1);
}

void	free_2d_char(char **array)
{
	int	i;

	i = 0;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	free_cmd_list(t_command **cmd_list)
{	printf("in free cmd list\n");
	t_command *tmp;

	while (*cmd_list)
	{
		tmp = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		// if (tmp->command)
		// 	free(tmp->command);
		if (tmp->args)
			free_2d_char(tmp->args);
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