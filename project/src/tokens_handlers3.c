/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handlers3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-20 20:55:09 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-20 20:55:09 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

Token	*create_token(char *value, Token_type type)
{
	Token	*token;

	token = malloc(sizeof(Token));
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
		if (strncmp(*env, var_name, var_name_len) == 0
			&& (*env)[var_name_len] == '=')
			return (*env + var_name_len + 1);
		env++;
	}
	return (NULL);
}

void	add_token(Token **head, Token **current, Token *token)
{
	if (!*head)
		*head = token;
	if (*current)
		(*current)->next = token;
	*current = token;
}

void	print_tokens(Token *tokens)
{
	while (tokens)
	{
		printf("Token: %s, Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

void	free_tokens(Token *tokens)
{
	Token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
