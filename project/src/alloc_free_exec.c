/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:13:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/07 13:08:12 by Watanudon        ###   ########.fr       */
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


int **alloc_fd(int nr_cmd)
{
	int **fd_pipe;
	int	i;

	//malloc space for nr of pointers to pipes
	fd_pipe = (int **)malloc((nr_cmd - 1) * sizeof(int *));
	if (!fd_pipe)
	{
		perror("malloc: ");
		return NULL;
	}
	i = 0;
	//in loop maloc space for each pipe
	while (i < nr_cmd - 1)
	{
		fd_pipe[i] = (int *)malloc(sizeof(int) * 2);
		if (!fd_pipe[i])
		{
			//free 2d array before 
			free_2d_array(fd_pipe, i - 1);
			perror("malloc: ");
			return NULL;
		}
		i++;
	}
	return (fd_pipe);
}

int *alloc_pid(int nr_cmd)
{
	int *pid;

	pid = (int *)malloc(sizeof(int) * nr_cmd);
	if (!pid)
	{
		//free everything
		perror("malloc: ");
<<<<<<< HEAD
		return NULL;
=======
		exit(348); //TODO
>>>>>>> minishell_dana/dana_parsing
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
<<<<<<< HEAD
		if (tmp->key)
		{
			free(tmp->key);
			tmp->key = NULL;
		}
		if (tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
=======
		free(tmp->key);
		tmp->key = NULL;
		free(tmp->value);
		tmp->value = NULL;
>>>>>>> minishell_dana/dana_parsing
		free(tmp);
	}
	*env = NULL;
}

void	free_pipe_array(int **fd_pipe, int nr_cmd)
{
	int	i;

	i = 0;
	 printf("in freeing fd array: nr cmd is %i\n", nr_cmd);
	while (i < nr_cmd - 1)// && fd_pipe[i])
	{
		free(fd_pipe[i]);
		fd_pipe[i] = NULL;
		i++;
	}
	free(fd_pipe);
	fd_pipe = NULL;
}

void	free_everything_pipeline_exit(t_env *envp, t_pipeline *pipeline, int stat)	//t_env *envp, int **fd_pipe, int *pid)
{
	printf("freeing pipeline\n");

	if (envp)
		free_env_list(&envp);
	printf("0\n");

	if (pipeline->fd_pipe)
		free_pipe_array(pipeline->fd_pipe, pipeline->nr_cmd);
	printf("1\n");

	if (pipeline->pid)
		free(pipeline->pid);
	printf("2\n");

	if (pipeline->env_array)
		free_2d_char(pipeline->env_array);
	printf("3\n");

	//free cmd_list if not statically alloced
	if (pipeline->cmd_list)
		free_cmd_list(&pipeline->cmd_list);
<<<<<<< HEAD
	printf("freed pipe\n");

	envp = NULL;
	pipeline = NULL;
				printf("now exiting\n");

	exit(stat);
=======
	envp = NULL;
	pipeline = NULL;
	exit(1);
>>>>>>> minishell_dana/dana_parsing
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
<<<<<<< HEAD
{	
	printf("in free cmd list\n");
=======
{	printf("in free cmd list\n");
>>>>>>> minishell_dana/dana_parsing
	t_command *tmp;

	while (*cmd_list)
	{
		//printf("in loop freeing\n");
		tmp = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		// if (tmp->command)
		// 	free(tmp->command);
		if (tmp->args)
			free_2d_char(tmp->args);
		//printf("0\n");
		if (tmp->input_file)
			free(tmp->input_file);
		//printf("1\n");
		if (tmp->output_file)
			free(tmp->output_file);
		//printf("2\n");

		if (tmp->exec_path)
			free(tmp->exec_path);
		//printf("3\n");

		free(tmp);
		//printf("4\n");
		tmp = NULL;
	}
	*cmd_list = NULL;
}