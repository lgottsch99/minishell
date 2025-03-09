/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:39:33 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/09 17:15:36 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	shlvl(t_env	*new_node, t_env *environ)
{
	if (ft_strncmp(new_node->key, "SHLVL", 5) == 0)
	{	
		printf("adding to shlvl\n");
		//get value,  atoi, +1, itoa
		new_node->value = add_shlvl(new_node->value); //MALLOC
		if (!new_node->value)
		{
			free_env_list(&environ);
			return (1);
		}
	}
	return (0);
}

char	*add_shlvl(char *value)
{
	int		num;
	
	num = ft_atoi(value);
	free(value);
	value = NULL;
	num = num + 1;
	return (ft_itoa(num));
}


int	set_more(char **equal, char *envp, t_env *new_node, t_env *environ)
{
	*equal = ft_strchr(envp, '=');//check if = in str
	if (!*equal)//no = in envp str
	{//assign str as key
		new_node->key = envp;
		new_node->value = NULL;
	}
	else //equal in str
	{
		if (set_key_value(envp, new_node, environ) == 1) //MALLOC
			return (1);
	}
	new_node->next = NULL;
	if (shlvl(new_node, environ) == 1) //if shlvl add 1
		return (1);
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