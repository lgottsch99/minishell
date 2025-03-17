/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:39:33 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 14:57:25 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*add_shlvl(char *value)
{
	int	num;

	num = ft_atoi(value);
	free(value);
	value = NULL;
	num = num + 1;
	return (ft_itoa(num));
}

static int	more_set_key_value(char *value, char *key, t_env *environ,
	t_env *new_node)
{
	char	*trimmed;

	if (value[0] == '"' && value[ft_strlen(value)] == '"')
	{
		trimmed = ft_strtrim(value, "\"");
		if (!trimmed)
		{
			free_env_list(&environ);
			return (1);
		}
		free(value);
		value = trimmed;
	}
	new_node->key = key;
	new_node->value = value;
	return (0);
}

int	set_key_value(char *env_str, t_env *new_node, t_env *environ)
{
	char	*key;
	char	*value;
	int		index_first;

	index_first = 0;
	while (env_str[index_first] != '=')
		index_first++;
	key = ft_substr(env_str, 0, index_first);
	if (!key)
	{
		free_env_list(&environ);
		return (1);
	}
	value = ft_substr(env_str, index_first + 1,
			ft_strlen(env_str) - 1 - index_first);
	if (!value)
	{
		free_env_list(&environ);
		return (1);
	}
	if (more_set_key_value(value, key, environ, new_node) == 1)
		return (1);
	return (0);
}

static int	set_more(char **equal, char *envp, t_env *new_node, t_env *environ)
{
	*equal = ft_strchr(envp, '=');
	if (!*equal)
	{
		new_node->key = envp;
		new_node->value = NULL;
	}
	else
	{
		if (set_key_value(envp, new_node, environ) == 1)
			return (1);
	}
	new_node->next = NULL;
	if (shlvl(new_node, environ) == 1)
		return (1);
	return (0);
}

t_env	*set_env(char *envp[])
{
	t_env	*environ;
	t_env	*new_node;
	char	*equal;
	int		i;

	environ = NULL;
	new_node = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = (t_env *)malloc(sizeof(t_env) * 1);
		if (!new_node)
		{
			free_env_list(&environ);
			return (NULL);
		}
		if (set_more(&equal, envp[i], new_node, environ) == 1)
			return (NULL);
		add_env_back(&environ, new_node);
		equal = NULL;
		new_node = NULL;
		i++;
	}
	return (environ);
}
