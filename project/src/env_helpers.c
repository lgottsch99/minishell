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

char **env_to_array(t_env *env) //dana ft
{
	int		count;
	int		i;
	int		j;
	t_env	*current;
	char	**env_array;
	size_t	len;

	if (!env)
		return (NULL);
	current = env;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc((count + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (i < count)
	{
		len = ft_strlen(current->key) + ft_strlen(current->value) + 2;
		env_array[i] = malloc(len);
		if (!env_array[i])
		{
			j = 0;
			while (j < i)
			{
				free(env_array[j]);
				j++;
			}
			free(env_array);
			return (NULL);
		}
		snprintf(env_array[i], len, "%s=%s", current->key, current->value); // forbidden, todo
		current = current->next;
		i++;
	}
	env_array[count] = NULL;
	return(env_array);
}

