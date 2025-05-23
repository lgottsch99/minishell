/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handlers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 14:14:46 by dvasilen          #+#    #+#             */
/*   Updated: 2025/03/25 17:17:45 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_delimeter(char **start, char **end)
{
	char	*delimiter;

	*start = *end + 2;
	while (**start && ft_isspace(**start))
		(*start)++;
	*end = *start;
	while (**end && !ft_isspace(**end) && **end != '\0')
		(*end)++;
	delimiter = ft_strndup(*start, *end - *start);
	return (delimiter);
}

void	handle_heredoc(char **start, char **end,
	t_token **head, t_token **current)
{
	t_token	*token;
	char	*delimiter;

	delimiter = extract_delimeter(start, end);
	if (!delimiter)
		return ;
	token = create_token(ft_strdup("<<"), TOKEN_REDIRECT_HEREDOC);
	if (!token)
	{
		free(delimiter);
		free(token->value);
		free(token);
		return ;
	}
	add_token(head, current, token);
	token = create_token(delimiter, TOKEN_WORD);
	if (!token)
	{
		free(delimiter);
		return ;
	}
	add_token(head, current, token);
	*start = *end;
}

void	handle_redir_less(char **start, char **end,
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
	if (*(*end + 1) == '<')
		handle_heredoc(start, end, head, current);
	else
	{
		token = create_token(ft_strdup("<"), TOKEN_REDIRECT_IN);
		add_token(head, current, token);
	}
	*start = ++(*end);
}

void	handle_word(t_tokenizeContext *token_ctx, t_envVarContext *env_ctx)
{
	char	*value;
	t_token	*token;

	while (**token_ctx->end && !ft_isspace(**token_ctx->end)
		&& **token_ctx->end != '|' && **token_ctx->end != '>'
		&& **token_ctx->end != '<' && **token_ctx->end != '='
		&& **token_ctx->end != '"' && **token_ctx->end != '\'')
		handle_env_var_in_word(token_ctx, env_ctx);
	if (**token_ctx->end == '=')
	{
		(*token_ctx->end)++;
		if (**token_ctx->end == '"' || **token_ctx->end == '\'')
			handle_quoted_value_after_equal(token_ctx->end);
		else
			handle_unquoted_value_after_equal(token_ctx->end);
		value = process_value_after_equal(token_ctx->start, token_ctx->end);
	}
	else
		value = ft_strndup(*token_ctx->start,
				*token_ctx->end - *token_ctx->start);
	token = create_token(value, TOKEN_WORD);
	add_token(token_ctx->head, token_ctx->current, token);
	*token_ctx->start = *token_ctx->end;
}
