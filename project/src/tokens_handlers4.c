/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handlers4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:44:47 by dvasilen          #+#    #+#             */
/*   Updated: 2025/04/06 15:18:04 by lgottsch         ###   ########.fr       */
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
	int		check;

	(*end)++;
	*start = *end;
	check = 0;
	while (**end && **end != '\'')
	{
		if (!ft_isspace(**end))
			check = 1;
		(*end)++;
	}
	if (*end != *start && check != 0)
	{
		value = ft_strndup(*start, *end - *start);
		token = create_token(value, TOKEN_WORD);
		add_token(head, current, token);
	}
	(*end)++;
	*start = *end;
}

void	handle_space(t_tdata *data)
{
	if (data->end > data->start)
		handle_word(&data->token_ctx, &data->env_ctx);
	data->start = ++data->end;
}

void	init_double_quote_handling(t_quote_context *ctx,
			int *buffer_index, int *check)
{
	*check = 0;
	(*ctx->end)++;
	*ctx->start = *ctx->end;
	*buffer_index = 0;
	if (**ctx->end == '"')
	{
		handle_empty_quotes(ctx);
		return ;
	}
}

void	process_double_quote_content(t_quote_context *ctx, char *buffer,
			int *buffer_index, int *check)
{
	while (**ctx->end && **ctx->end != '"')
	{
		if (**ctx->end == '$')
			process_env_var(ctx, buffer, buffer_index);
		else
		{
			if (!ft_isspace(**ctx->end))
				*check = 1;
			(*ctx->end)++;
		}
	}
}
