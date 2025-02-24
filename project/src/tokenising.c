/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenising.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-20 19:02:32 by dvasilen          #+#    #+#             */
/*   Updated: 2025-02-20 19:02:32 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

Token	*create_token(char *value, Token_type type)
{
	Token	*token;

	token = malloc(sizeof(Token));
	if (!token)
		return NULL;
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
		if (strncmp(*env, var_name, var_name_len) == 0 && (*env)[var_name_len] == '=')
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

void	handle_pipe(char **start, char **end, Token **head, Token **current)
{
	Token	*token;
	if (*end > *start)
	{
		char *value = ft_strndup(*start, *end - *start);
		token = create_token(value, TOKEN_WORD);
		add_token(head, current, token);
	}
	token = create_token(ft_strdup("|"), TOKEN_PIPE);
	*start = ++(*end);
	add_token(head, current, token);
}

void	handle_single_quote(char **start, char **end, Token **head, Token **current)
{
	(*end)++;
	*start = *end;
	while(**end && **end != '\'')
		(*end)++;
	char *value = ft_strndup(*start, *end - *start);
	Token *token = create_token(value, TOKEN_WORD);
	add_token(head, current, token);
	(*end)++;
	*start = *end;
}

void	handle_double_quote(char **start, char **end, Token **head, Token **current)
{
	char	*value;
	Token	*token;
	(*end)++;
	*start = *end;
	while(**end && **end != '"')
		(*end)++;
	value = ft_strndup(*start, *end - *start);
	token = create_token(value, TOKEN_WORD);
	add_token(head, current, token);
	(*end)++;
	*start = *end;
}

void	handle_env_var(char **start, char **end, Token **head, Token **current, int last_exit_status, char **envp) 
{
	Token	*token;
	char	*exit_status_str;
	char	*var_start;
	char	*var_name;
	char	*var_value;

	(*end)++;
	var_start = *end;
	if (*var_start == '?')
	{
		exit_status_str = malloc(12);
		if (!exit_status_str)
		{
			fprintf(stderr, "Error: malloc failed\n");
			return;
		}
		snprintf(exit_status_str, 12, "%d", last_exit_status);
		token = create_token(exit_status_str, TOKEN_WORD);
		if (!token)
		{
			free(exit_status_str);
			fprintf(stderr, "Error: create_token failed\n");
			return;
		}
		add_token(head, current, token);
		(*end)++;
		*start = *end;
		return;
	}
	while (**end && (**end == '_' || ft_isalnum(**end)))
		(*end)++;
	var_name = ft_strndup(var_start, *end - var_start);
	if (!var_name)
		return;
	var_value = ft_getenv(var_name, envp);
	if (var_value)
		token = create_token(ft_strdup(var_value), TOKEN_WORD);
	else
		token = create_token(ft_strdup(""), TOKEN_WORD);
	if (!token)
	{
		free(var_name);
		return;
	}
	add_token(head, current, token);
	free(var_name);
	*start = *end;
}

void	handle_redir_more(char **start, char **end, Token **head, Token **current) {
	Token *token;
	if (*end > *start)
	{
		char *value = ft_strndup(*start, *end - *start);
		token = create_token(value, TOKEN_WORD);
		add_token(head, current, token);
	}
	if (*(*end + 1) == '>')
	{
		token = create_token(ft_strdup(">>"), TOKEN_REDIRECT_APPEND);
		add_token(head, current, token);
		(*end)++;
	} 
	else
	{
		token = create_token(ft_strdup(">"), TOKEN_REDIRECT_OUT);
		add_token(head, current, token);
	}
	*start = ++(*end);
}

void	handle_redir_less(char **start, char **end, Token **head, Token **current) {
	Token	*token;
	if (*end > *start)
	{
		char *value = ft_strndup(*start, *end - *start);
		token = create_token(value, TOKEN_WORD);
		add_token(head, current, token);
	}
	if (*(*end + 1) == '<')
	{
		token = create_token(ft_strdup("<<"), TOKEN_REDIRECT_HEREDOC);
		add_token(head, current, token);
		(*end)++;
	}
	else
	{
		token = create_token(ft_strdup("<"), TOKEN_REDIRECT_IN);
		add_token(head, current, token);
	}
	*start = ++(*end);
}

Token	*tokenize(char *input, int last_exit_status, char **envp)
{
	Token *head = NULL;
	Token *current = NULL;
	Token *token = NULL;
	char *start = input;
	char *end = input;
	char *value;

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
		else if (*end == '"')
			handle_double_quote(&start, &end, &head, &current);
		else if (*end == '\'')
			handle_single_quote(&start, &end, &head, &current);
		else if (*end == '$')
			handle_env_var(&start, &end, &head, &current, last_exit_status, envp);
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