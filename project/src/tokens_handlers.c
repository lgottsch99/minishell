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
	Token **head, Token **current, int last_exit_status, char **envp)
{
	Token			*token;
	char			buffer[1024];
	int				buffer_index;
	char			*value;
	EnvVarContext	ctx;

	(*end)++;
	*start = *end;
	if (**end == '"')
	{
		while (**end == '"')
			(*end)++;
		if (*current && (*current)->type == TOKEN_WORD)
		{
		}
		else
		{
			token = create_token(ft_strdup(""), TOKEN_WORD);
			add_token(head, current, token);
		}
		*start = *end;
		return ;
	}
	buffer_index = 0;
	while (**end && **end != '"')
	{
		if (**end == '$')
		{
			if (*end > *start)
			{
				strncpy(buffer + buffer_index, *start, *end - *start);
				buffer_index += (*end - *start);
			}
			ctx = (EnvVarContext){start, end, head, current,
				last_exit_status, envp, 0};
			value = handle_env_var(&ctx);
			if (value)
			{
				strncpy(buffer + buffer_index, value, ft_strlen(value));
				buffer_index += ft_strlen(value);
				free(value);
			}
			*start = *end;
		}
		else
			(*end)++;
	}
	if (*end > *start)
	{
		strncpy(buffer + buffer_index, *start, *end - *start);
		buffer_index += *end - *start;
	}
	buffer[buffer_index] = '\0';
	token = create_token(ft_strdup(buffer), TOKEN_WORD);
	add_token(head, current, token);
	(*end)++;
	*start = *end;
	while (**end == '"')
		(*end)++;
}
