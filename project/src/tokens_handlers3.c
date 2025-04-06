/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handlers3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 20:55:09 by dvasilen          #+#    #+#             */
/*   Updated: 2025/04/06 14:08:41 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;
	int		y;
	size_t	i;

	y = 0;
	i = 0;
	while (i < ft_strlen(value))
	{
		if (ft_isspace(value[i]) == 0)
			y = 1;
		i++;
	}
	if (y == 0)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

char	*ft_getenv(char *var_name, char **envp)
{
	char	**env;
	int		var_name_len;

	if (!var_name || !*var_name || !envp)
		return (NULL);
	var_name_len = ft_strlen(var_name);
	env = envp;
	while (*env)
	{
		if (ft_strncmp(*env, var_name, var_name_len) == 0
			&& (*env)[var_name_len] == '=')
			return (*env + var_name_len + 1);
		env++;
	}
	return (NULL);
}

void	add_token(t_token **head, t_token **current, t_token *token)
{
	if (!*head)
		*head = token;
	if (*current)
		(*current)->next = token;
	*current = token;
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("t_token: %s, Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
