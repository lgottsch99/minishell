/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 14:05:35 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/15 15:54:16 by lgottsch         ###   ########.fr       */
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
	while(current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc((count + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while ( i < count)
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
		snprintf(env_array[i], len, "%s=%s", current->key, current->value); //toooodoooo
		current = current->next;
		i++;
	}
	env_array[count] = NULL;
	return(env_array);
}



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

//goes thru our own env and searches for var for given key, if it doesnt exist returns NULL
char	*ret_value_env(char *key, t_env *envp)
{
	printf("searching env\n");
	printf("key: %s\n", key);
	
	t_env	*tmp;

	if (!key || !envp)
		return NULL;

	tmp = envp;
	//go thru list until we find key
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key)) == 0)
			return ((char *)tmp->value);	
		tmp = tmp->next;
	}
	printf("no key in env found\n");
	return (NULL);
}

char **convert_env_array(t_env *envp, t_pipeline *pipeline) //lilli ft //The envp array must be terminated by a NULL pointer.
{
	char	**array;
	int		lstsize;
	t_env	*tmp;
	int		i;
	char	*fullstr;

	//count size list
	lstsize = count_env_size(envp);
	//printf("size list is: %i\n", lstsize);

	//malloc space for pointer array + 1 for NULL
	array = (char **)malloc(sizeof(char *) * (lstsize + 1));
	if (!array)
		return (NULL);
	
	//go thru list and array
	tmp = envp;
	i = 0;
	while(tmp && i < lstsize)
	{
		//for each node in envp: 
			//create full string in form key=value
		fullstr = create_fullstr(tmp); //MALLOC
		if (!fullstr)
		{
			free_2d_char(array);
			free_everything_pipeline_exit(envp, pipeline, 1);
		}
		//set full str to array[i]
		array[i] = fullstr;
		tmp = tmp->next;
		i++;
	}
	array[i] = NULL;

	// for (int y = 0; y <= lstsize; y++) { //debug only
    // printf("envp[%i]:%s\n", y, array[y]);
	// }

	return (array);
}
