/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:13:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/07 16:16:25 by lgottsch         ###   ########.fr       */
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
}


int **alloc_fd(int nr_cmd)
{
	int **fd_pipe;
	int	i;

	i = 0;
	//malloc space for nr of pointers to pipes
	fd_pipe = (int **)malloc((nr_cmd - 1) * sizeof(int *));
	if (!fd_pipe)
	{
		//free everything
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
			//free everything else 
			perror("malloc: ");
			exit(777);
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
		exit(348);
	}
	return (pid);
}
