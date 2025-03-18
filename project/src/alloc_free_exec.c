/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:13:22 by lgottsch          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/03/17 19:29:22 by lgottsch         ###   ########.fr       */
=======
/*   Updated: 2025/03/08 18:51:02 by lgottsch         ###   ########.fr       */
>>>>>>> dana
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	**alloc_fd(int nr_cmd)
{
<<<<<<< HEAD
	int	**fd_pipe;
	int	i;

=======
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
>>>>>>> dana
	fd_pipe = (int **)malloc((nr_cmd - 1) * sizeof(int *));
	if (!fd_pipe)
	{
		perror("malloc: ");
<<<<<<< HEAD
		return (NULL);
	}
	i = 0;
=======
		return NULL;
	}
	i = 0;
	//in loop maloc space for each pipe
>>>>>>> dana
	while (i < nr_cmd - 1)
	{
		fd_pipe[i] = (int *)malloc(sizeof(int) * 2);
		if (!fd_pipe[i])
		{
<<<<<<< HEAD
			free_2d_array(fd_pipe, i - 1);
			perror("malloc: ");
			return (NULL);
=======
			//free 2d array before 
			free_2d_array(fd_pipe, i - 1);
			perror("malloc: ");
			return NULL;
>>>>>>> dana
		}
		i++;
	}
	return (fd_pipe);
}

<<<<<<< HEAD
int	*alloc_pid(int nr_cmd)
=======
int *alloc_pid(int nr_cmd)
>>>>>>> dana
{
	int	*pid;

	pid = (int *)malloc(sizeof(int) * nr_cmd);
	if (!pid)
	{
<<<<<<< HEAD
		perror("malloc: ");
		return (NULL);
=======
		//free everything
		perror("malloc: ");
		return NULL;
>>>>>>> dana
	}
	return (pid);
}

<<<<<<< HEAD
void	free_everything_pipeline_exit(t_env *envp,
	t_pipeline *pipeline, int stat)
=======
void	free_env_list(t_env **env)
{
	t_env *tmp;

	while (*env)
	{
		tmp = *env;
		*env = (*env)->next;
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
>>>>>>> dana
{
	printf("freeing pipeline\n");

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
<<<<<<< HEAD
}

void	free_cmd_list(t_command **cmd_list)
{
	t_command	*tmp;
=======
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
{	
	printf("in free cmd list\n");
	t_command *tmp;
>>>>>>> dana

	while (*cmd_list)
	{
		//printf("in loop freeing\n");
		tmp = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		if (tmp->args)
			free_2d_char(tmp->args);
<<<<<<< HEAD
		remove_heredoc(tmp->heredoc_file);//??ok
		if (tmp->heredoc_delimetr)
			free(tmp->heredoc_delimetr);
=======
		if (tmp->heredoc_file)
		 	free (tmp->heredoc_file);
		if (tmp->heredoc_delimetr)
			free (tmp->heredoc_delimetr);
>>>>>>> dana
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