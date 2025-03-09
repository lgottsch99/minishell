/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenising.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:02:32 by dvasilen          #+#    #+#             */
/*   Updated: 2025/03/07 13:20:46 by Watanudon        ###   ########.fr       */
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

Token	*tokenize(char *input, int last_exit_status, char **envp)
{
	Token	*head;
	Token	*current;
	Token	*token;
	char	*start;
	char	*end;
	char	*value;
	char	*quoted_value;

	head = NULL;
	current = NULL;
	token = NULL;
	start = input;
	end = input;
	while (*end)
	{
		if (ft_isspace(*end))
		{
			if (end > start)
			{
				value = ft_strndup(start, end - start);
				token = create_token(value, TOKEN_WORD);
				add_token(&head, &current, token);
			}
			start = ++end;
		}
		else if (*end == '|')
			handle_pipe(&start, &end, &head, &current);
		else if (*end == '>')
			handle_redir_more(&start, &end, &head, &current);
		else if (*end == '<')
			handle_redir_less(&start, &end, &head, &current);
		// else if (*end == '"')
		// 	handle_double_quote(&start, &end, &head, &current);
		// else if (*end == '\'')
		// 	handle_single_quote(&start, &end, &head, &current);
		else if (*end == '"' || *end == '\'')
		{
			if (end > start)
			{
				value = ft_strndup(start, end - start);
				token = create_token(value, TOKEN_WORD);
				add_token(&head, &current, token);
			}
			quoted_value = handle_quotes(&start, &end, *end);
			token = create_token(quoted_value, TOKEN_WORD);
			add_token(&head, &current, token);
			start = end;
		}
		else if (*end == '$')
			handle_env_var(&start, &end, &head, &current,
				last_exit_status, envp);
		else
			end++;
	}
	if (start != end)
	{
		value = ft_strndup(start, end - start);
		token = create_token(value, TOKEN_WORD);
		add_token(&head, &current, token);
	}
	token = create_token(NULL, TOKEN_END);
	add_token(&head, &current, token);
	return (head);
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
