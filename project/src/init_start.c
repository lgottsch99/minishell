/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:43:57 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/07 14:04:38 by Watanudon        ###   ########.fr       */
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
	//printf("\n");
	//printf("Welcome to our minishell!\n");
	printf("\n");
	printf("\n");
	return;
}

char	*add_shlvl(char *value)
{
	int		num;
	//char	*new;
	//atoi, add 1, itoa, return
	num = ft_atoi(value);
	free(value);
	value = NULL;
	
	num = num + 1;
	// new = ft_itoa(num); //MALLOC
	// if (!new)
	// 	return (NULL);
	//return (new);
	return (ft_itoa(num));
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

void	add_env_back(t_env **environ, t_env *new_node)//add new node to end of list
{
	t_env	*last;
	//go to end of list
	if (*environ == NULL)
	{
		*environ = new_node;
		return ;
	}
	last = *environ;
	while (last->next)
		last = last->next;
	//set last node ->next -> new node
	last->next = new_node;
}


t_env	*set_env(char *envp[]) //create linked list w all env vars, increasing shlvl by 1
{
	printf("creating environ\n");
	t_env	*environ;
	t_env	*new_node;
	char	*equal;
	int		i;

	environ = NULL;
	new_node = NULL;
	i = 0;
	//loop thru envp
	while (envp[i])
	{//malloc space for node
		new_node = (t_env *)malloc(sizeof(t_env) * 1);
		if (!new_node)
		{
			free_env_list(&environ);
			//free anything else?
			return (NULL);
		}
	//check if = in str
		equal = ft_strchr(envp[i], '=');
		if (!equal)//no = in envp str
		{//assign str as key
			new_node->key = envp[i];
			new_node->value = NULL;
		}
		else //equal in str
		{
			if (set_key_value(envp[i], new_node, environ) == 1) //MALLOC
				return (NULL);
		}
		new_node->next = NULL;
		//if shlvl add 1
		if (ft_strncmp(new_node->key, "SHLVL", 5) == 0)
		{	printf("adding to shlvl\n");
			//get value,  atoi, +1, itoa
			new_node->value = add_shlvl(new_node->value); //MALLOC
			if (!new_node->value)
			{
				free_env_list(&environ);
			//free anything else?
				return (NULL);
			}
		}
		//connect to list
		add_env_back(&environ, new_node);
		equal = NULL;
		new_node = NULL;
		i++;
	}
	return (environ);
}

