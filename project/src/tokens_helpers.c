/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-09 14:14:46 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-09 14:14:46 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_heredoc(char **start, char **end,
	Token **head, Token **current)
{
	Token	*token;
	char	*delimiter;

	*start = *end + 2;
	while (**start && ft_isspace(**start))
		(*start)++;
	*end = *start;
	while (**end && !ft_isspace(**end) && **end != '\0')
		(*end)++;
	delimiter = ft_strndup(*start, *end - *start);
	if (!delimiter)
	{
		fprintf(stderr, "Error: malloc failed\n");
		return ;
	}
	token = create_token(ft_strdup("<<"), TOKEN_REDIRECT_HEREDOC);
	if (!token)
	{
		free(delimiter);
		fprintf(stderr, "Error: create_token failed\n");
		return ;
	}
	add_token(head, current, token);
	token = create_token(delimiter, TOKEN_WORD);
	if (!token)
	{
		free(delimiter);
		fprintf(stderr, "Error: create_token failed\n");
		return ;
	}
	add_token(head, current, token);
	*start = *end;
}

void	handle_redir_less(char **start, char **end,
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
	if (*(*end + 1) == '<')
	{
		handle_heredoc(start, end, head, current);
	}
	else
	{
		token = create_token(ft_strdup("<"), TOKEN_REDIRECT_IN);
		add_token(head, current, token);
	}
	*start = ++(*end);
}

void	handle_word(char **start, char **end, Token **head, Token **current, int last_exit_status, char **envp)
{
	char	*value;
	char	*equal_sign;
	char	*quoted_value;
	Token	*token;

	while (**end && !ft_isspace(**end) && **end != '|' && **end != '>' && **end != '<' && **end != '=')
	{
		if (**end == '$')
		{
			handle_env_var(start, end, head, current, last_exit_status, envp, 1);
			*start = *end;
		}
		else
			(*end)++;
    }
	if (**end == '=')
	{
		(*end)++;
		if (**end == '"' || **end == '\'')
		{
			char quote = **end;
			(*end)++;
			while (**end && **end != quote)
				(*end)++;
			if (**end == quote)
				(*end)++;
		}
		else
		{
			while (**end && !ft_isspace(**end) && **end != '|' && **end != '>' && **end != '<')
				(*end)++;
		}	
		value = ft_strndup(*start, *end - *start);
		equal_sign = ft_strchr(value, '=');
		if (equal_sign && (*(equal_sign + 1) == '"' || *(equal_sign + 1) == '\''))
		{
			quoted_value = equal_sign + 2;
			if (quoted_value[ft_strlen(quoted_value) - 1] == '"' || quoted_value[ft_strlen(quoted_value) - 1] == '\'')
				quoted_value[ft_strlen(quoted_value) - 1] = '\0';
			ft_strlcpy(equal_sign + 1, quoted_value, ft_strlen(quoted_value) + 1);
		}
	}
	else
		value = ft_strndup(*start, *end - *start);
	token = create_token(value, TOKEN_WORD);
	add_token(head, current, token);
	*start = *end;
}