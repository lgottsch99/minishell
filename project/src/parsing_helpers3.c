/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-03 18:14:21 by dvasilen          #+#    #+#             */
/*   Updated: 2025-04-03 18:14:21 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_word_token(t_token **token, t_parse_ctx *ctx)
{
	if (!handle_word_parsing(*(ctx->cmd), (*token)->value, ctx->head))
		return (0);
	return (1);
}

int	handle_pipe_token(t_parse_ctx *ctx)
{
	handle_pipe_parsing(ctx->head, ctx->current, ctx->cmd, ctx->count);
	return (1);
}

int	handle_redir_token(t_token **token, t_parse_ctx *ctx, int is_output)
{
	if (!handle_redirection_parsing(token, ctx, is_output))
		return (0);
	return (1);
}

int	process_token(t_token **token, t_parse_ctx *ctx)
{
	if (!*token)
		return (0);
	if ((*token)->type == TOKEN_REDIRECT_HEREDOC)
		return (handle_heredoc_token(token, ctx));
	else if ((*token)->type == TOKEN_WORD)
		return (handle_word_token(token, ctx));
	else if ((*token)->type == TOKEN_PIPE)
		return (handle_pipe_token(ctx));
	else if ((*token)->type == TOKEN_REDIRECT_IN)
		return (handle_redir_token(token, ctx, 0));
	else if ((*token)->type == TOKEN_REDIRECT_OUT
		|| (*token)->type == TOKEN_REDIRECT_APPEND)
		return (handle_redir_token(token, ctx, 1));
	return (1);
}
