/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-09 13:55:16 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-09 13:55:16 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_pipe(char **start, char **end, Token **head, Token **current)
{
	Token	*token;
	char	*value;

	if (*end > *start)
	{
		value = ft_strndup(*start, *end - *start);
		token = create_token(value, TOKEN_WORD);
		add_token(head, current, token);
	}
	token = create_token(ft_strdup("|"), TOKEN_PIPE);
	*start = ++(*end);
	add_token(head, current, token);
}

void	handle_single_quote(char **start, char **end,
	Token **head, Token **current)
{
	char	*value;
	Token	*token;

	(*end)++;
	*start = *end;
	while (**end && **end != '\'')
		(*end)++;
	value = ft_strndup(*start, *end - *start);
	token = create_token(value, TOKEN_WORD);
	add_token(head, current, token);
	(*end)++;
	*start = *end;
}

void	handle_double_quote(char **start, char **end,
	Token **head, Token **current)
{
	char	*value;
	Token	*token;

	(*end)++;
	*start = *end;
	while (**end && **end != '"')
		(*end)++;
	value = ft_strndup(*start, *end - *start);
	token = create_token(value, TOKEN_WORD);
	add_token(head, current, token);
	(*end)++;
	*start = *end;
}

void	handle_env_var(char **start, char **end, Token **head,
	Token **current, int last_exit_status, char **envp)
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
			return ;
		}
		snprintf(exit_status_str, 12, "%d", last_exit_status);
		token = create_token(exit_status_str, TOKEN_WORD);
		if (!token)
		{
			free(exit_status_str);
			fprintf(stderr, "Error: create_token failed\n");
			return ;
		}
		add_token(head, current, token);
		(*end)++;
		*start = *end;
		return ;
	}
	while (**end && (**end == '_' || ft_isalnum(**end)))
		(*end)++;
	var_name = ft_strndup(var_start, *end - var_start);
	if (!var_name)
		return ;
	var_value = ft_getenv(var_name, envp);
	if (var_value)
		token = create_token(ft_strdup(var_value), TOKEN_WORD);
	else
		token = create_token(ft_strdup(""), TOKEN_WORD);
	if (!token)
	{
		free(var_name);
		return ;
	}
	add_token(head, current, token);
	free(var_name);
	*start = *end;
}

void	handle_redir_more(char **start, char **end,
	Token **head, Token **current)
{
	Token	*token;
	char	*value;

	if (*end > *start)
	{
		value = ft_strndup(*start, *end - *start);
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
