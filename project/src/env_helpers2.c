/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:54:31 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 15:04:40 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_env_size(t_env *envp)
{
	int		size;
	t_env	*tmp;

	tmp = envp;
	size = 0;
	if (envp)
	{
		size = 1;
		while (tmp->next)
		{
			tmp = tmp->next;
			size++;
		}
	}
	return (size);
}

//goes thru our own env and searches for var for given key, if it 
// doesnt exist returns NULL
char	*ret_value_env(char *key, t_env *envp)
{
	t_env	*tmp;

	if (!key || !envp)
		return (NULL);
	tmp = envp;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key)) == 0)
			return ((char *)tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

//lilli ft
char	**convert_env_array(t_env *envp, t_pipeline *pipeline)
{
	char	**array;
	t_env	*tmp;
	int		i;
	char	*fullstr;

	array = (char **)malloc(sizeof(char *) * (count_env_size(envp) + 1));
	if (!array)
		return (NULL);
	tmp = envp;
	i = 0;
	while (tmp && i < count_env_size(envp))
	{
		fullstr = create_fullstr(tmp);
		if (!fullstr)
		{
			free_2d_char(array);
			free_everything_pipeline_exit(envp, pipeline, 1);
		}
		array[i] = fullstr;
		tmp = tmp->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	add_env_back(t_env **environ, t_env *new_node)
{
	t_env	*last;

	if (*environ == NULL)
	{
		*environ = new_node;
		return ;
	}
	last = *environ;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

int	shlvl(t_env	*new_node, t_env *environ)
{
	if (ft_strncmp(new_node->key, "SHLVL", 5) == 0)
	{
		new_node->value = add_shlvl(new_node->value);
		if (!new_node->value)
		{
			free_env_list(&environ);
			return (1);
		}
	}
	return (0);
}
