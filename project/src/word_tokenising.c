/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_tokenising.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-20 22:01:58 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-20 22:01:58 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_env_var_in_word(TokenizeContext *token_ctx,
			EnvVarContext *env_ctx)
{
	char	*value;

	if (**token_ctx->end == '$')
	{
		value = handle_env_var(env_ctx);
		*token_ctx->start = *token_ctx->end;
	}
	else
		(*token_ctx->end)++;
}

void	handle_quoted_value_after_equal(char **end)
{
	char	quote;

	quote = **end;
	(*end)++;
	while (**end && **end != quote)
		(*end)++;
	if (**end == quote)
		(*end)++;
}

void	handle_unquoted_value_after_equal(char **end)
{
	while (**end && !ft_isspace(**end) && **end != '|'
		&& **end != '>' && **end != '<')
		(*end)++;
}

char	*process_value_after_equal(char **start, char **end)
{
	char	*value;
	char	*equal_sign;
	char	*quoted_value;

	value = ft_strndup(*start, *end - *start);
	equal_sign = ft_strchr(value, '=');
	if (equal_sign && (*(equal_sign + 1) == '"' || *(equal_sign + 1) == '\''))
	{
		quoted_value = equal_sign + 2;
		if (quoted_value[ft_strlen(quoted_value) - 1] == '"'
			|| quoted_value[ft_strlen(quoted_value) - 1] == '\'')
			quoted_value[ft_strlen(quoted_value) - 1] = '\0';
		ft_strlcpy(equal_sign + 1, quoted_value, ft_strlen(quoted_value) + 1);
	}
	return (value);
}
