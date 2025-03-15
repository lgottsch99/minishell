/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:42:18 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/15 15:44:38 by lgottsch         ###   ########.fr       */
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

void	free_2d_char(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	if (array)
		free(array);
	array = NULL;
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
