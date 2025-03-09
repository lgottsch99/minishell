/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:43:57 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/09 17:15:44 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void print_start(void)
{
	printf("\n");
	printf(" .▄▄ ·  ▄ .▄▄▄▄ .▄▄▌  ▄▄▌      ▄▄▌ ▐ ▄▌▄▄▄ .\n");
	printf(" ▐█ ▀. ██▪▐█▀▄.▀·██•  ██•      ██· █▌▐█▀▄.▀·\n");
	printf(" ▄▀▀▀█▄██▀▐█▐▀▀▪▄██▪  ██▪      ██▪▐█▐▐▌▐▀▀▪▄\n");
	printf(" ▐█▄▪▐███▌▐▀▐█▄▄▌▐█▌▐▌▐█▌▐▌    ▐█▌██▐█▌▐█▄▄▌\n");
	printf("  ▀▀▀▀ ▀▀▀ · ▀▀▀ .▀▀▀ .▀▀▀      ▀▀▀▀ ▀▪ ▀▀▀  ??????\n");
	printf("\n");
	printf("\n");
	return;
}



int	set_key_value(char *env_str, t_env *new_node, t_env *environ) // = in str
{
	char	*key;
	char	*value;
	char	*trimmed;
	int		index_first;

	//split by =
	index_first = 0;
	while (env_str[index_first] != '=')
		index_first++;
	key = ft_substr(env_str, 0, index_first); //MALLOC
	if(!key)
	{
		free_env_list(&environ);
		//fee sth else?
		return (1);
	}
	value = ft_substr(env_str, index_first + 1, ft_strlen(env_str) - 1 - index_first); //MALLOC
	//printf("value: %s\n", value);
	if(!value)
	{
		free_env_list(&environ);
		//fee sth else?
		return (1);
	}
//for value trim " " if there
	if (value[0] == '"' && value[ft_strlen(value)] == '"')
	{
		trimmed = ft_strtrim(value, "\""); //MALLOC
		if (!trimmed)
		{
			free_env_list(&environ);
		//fee sth else?
			return (1);
		}
		free(value);
		value = trimmed;
	}
	//set in new node
	new_node->key = key;
	new_node->value = value;
	return (0);
}



t_env	*set_env(char *envp[]) //create linked list w all env vars, increasing shlvl by 1
{printf("creating environ\n");
	t_env	*environ;
	t_env	*new_node;
	char	*equal;
	int		i;

	environ = NULL;
	new_node = NULL;
	i = 0;
	while (envp[i]) //loop thru envp
	{
		new_node = (t_env *)malloc(sizeof(t_env) * 1); //malloc space for node
		if (!new_node)
		{
			free_env_list(&environ);
			return (NULL);
		}
		if (set_more(&equal, envp[i], new_node, environ) == 1)
			return (NULL);
		
		add_env_back(&environ, new_node);//connect to list
		equal = NULL;
		new_node = NULL;
		i++;
	}
	return (environ);
}

