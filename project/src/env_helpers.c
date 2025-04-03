/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 14:05:35 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/25 15:38:33 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	free_env_array(char **array, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static char	*create_env_entry(t_env *node)
{
	char	*entry;
	size_t	key_len;
	size_t	val_len;
	size_t	total_len;

	key_len = ft_strlen(node->key);
	val_len = ft_strlen(node->value);
	total_len = key_len + val_len + 2;
	entry = malloc(total_len);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, node->key, total_len);
	ft_strlcat(entry, "=", total_len);
	ft_strlcat(entry, node->value, total_len);
	return (entry);
}

char	**env_to_array(t_env *env)
{
	char	**env_array;
	int		count;
	int		i;

	if (!env)
		return (NULL);
	count = count_env_nodes(env);
	env_array = malloc((count + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < count)
	{
		env_array[i] = create_env_entry(env);
		if (!env_array[i])
		{
			free_env_array(env_array, i);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	env_array[count] = NULL;
	return (env_array);
}
