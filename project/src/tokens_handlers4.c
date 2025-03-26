/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handlers4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-20 21:44:47 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-20 21:44:47 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_env_var(t_envVarContext *ctx)
{
	char	*exit_status_str;
	char	*value_copy;

	(*ctx->end)++;
	if (**ctx->end == '?')
	{
		exit_status_str = generate_exit_status_str(ctx->last_exit_status);
		if (!exit_status_str)
			return (NULL);
		update_pointers_after_exit(ctx->start, ctx->end);
		return (handle_token_creation(ctx->head, ctx->current,
				exit_status_str, ctx->create_flag));
	}
	else
	{
		value_copy = extract_var_name_and_value(ctx->end, ctx->envp);
		if (!value_copy)
			return (NULL);
		return (handle_token_creation(ctx->head, ctx->current,
				value_copy, ctx->create_flag));
	}
}

void	handle_single_quote(char **start, char **end,
	t_token **head, t_token **current)
{
	char	*value;
	t_token	*token;

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
