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

void	handle_pipe(char **start, char **end, t_token **head, t_token **current)
{
	t_token	*token;
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
	t_token **head, t_token **current)
{
	t_token	*token;
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

static void	handle_empty_quotes(t_quote_context *ctx)
{
	t_token	*token;

	while (*(*ctx->end) == '"')
		(*ctx->end)++;
	if (!(*ctx->current) || (*ctx->current)->type != TOKEN_WORD)
	{
		token = create_token(ft_strdup(""), TOKEN_WORD);
		add_token(ctx->head, ctx->current, token);
	}
	*ctx->start = *ctx->end;
}

static void	process_env_var(t_quote_context *ctx, char *buffer, int *buf_idx)
{
	char			*value;
	t_envVarContext	env_ctx;

	env_ctx = (t_envVarContext)
	{ctx->start, ctx->end, ctx->head, ctx->current,
		ctx->last_exit_status, ctx->envp, 0};
	if (*ctx->end > *ctx->start)
	{
		ft_strncpy(buffer + *buf_idx, *ctx->start, *ctx->end - *ctx->start);
		*buf_idx += (*ctx->end - *ctx->start);
	}
	value = handle_env_var(&env_ctx);
	if (value)
	{
		ft_strncpy(buffer + *buf_idx, value, ft_strlen(value));
		*buf_idx += ft_strlen(value);
		free(value);
	}
	*ctx->start = *ctx->end;
}

void	handle_double_quote(t_quote_context *ctx)
{
	char	buffer[1024];
	int		buffer_index;

	(*ctx->end)++;
	*ctx->start = *ctx->end;
	if (**ctx->end == '"')
		return (handle_empty_quotes(ctx));
	buffer_index = 0;
	while (**ctx->end && **ctx->end != '"')
	{
		if (**ctx->end == '$')
			process_env_var(ctx, buffer, &buffer_index);
		else
			(*ctx->end)++;
	}
	if (*ctx->end > *ctx->start)
		ft_strncpy(buffer + buffer_index, *ctx->start, *ctx->end - *ctx->start);
	buffer[buffer_index + (*ctx->end - *ctx->start)] = '\0';
	add_token(ctx->head, ctx->current,
		create_token(ft_strdup(buffer), TOKEN_WORD));
	(*ctx->end)++;
	*ctx->start = *ctx->end;
	while (**ctx->end == '"')
		(*ctx->end)++;
}
