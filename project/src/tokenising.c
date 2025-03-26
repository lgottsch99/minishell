/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenising.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:02:32 by dvasilen          #+#    #+#             */
/*   Updated: 2025/03/18 15:38:48 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*tokenize(char *input, int last_exit_status, char **envp)
{
	t_token			*head;
	t_token			*current;
	t_token			*token;
	char			*start;
	char			*end;
	t_envVarContext	env_ctx;
	t_tokenizeContext	token_ctx = {&start, &end, &head, &current};
	t_quote_context	quote_ctx;

	head = NULL;
	current = NULL;
	token = NULL;
	start = input;
	end = input;
	while (ft_isspace(*start))
		start++;
	end = start;
	if (!*start)
	{
		token = create_token(NULL, TOKEN_END);
		add_token(&head, &current, token);
		return (NULL);
	}
	while (*end)
	{
		if (ft_isspace(*end))
		{
			if (end > start)
				handle_word(&token_ctx, &env_ctx);
			start = ++end;
		}
		else if (*end == '|')
			handle_pipe(&start, &end, &head, &current);
		else if (*end == '>')
			handle_redir_more(&start, &end, &head, &current);
		else if (*end == '<')
			handle_redir_less(&start, &end, &head, &current);
		else if (*end == '"' || *end == '\'')
		{
			if (end > start)
				handle_word(&token_ctx, &env_ctx);
			if (*end == '"')
			{
				quote_ctx = (t_quote_context)
				{
					.start = &start,
					.end = &end,
					.head = &head,
					.current = &current,
					.last_exit_status = last_exit_status,
					.envp = envp
				};
				handle_double_quote(&quote_ctx);
			}
			else
				handle_single_quote(&start, &end, &head, &current);
		}
		else if (*end == '$')
		{
			env_ctx = (t_envVarContext){&start, &end, &head,
				&current, last_exit_status, envp, 1};
			handle_env_var(&env_ctx);
			while (*end && !ft_isspace(*end) && *end != '|'
				&& *end != '>' && *end != '<' && *end != '=')
				(*end)++;
			start = end;
		}
		else
			handle_word(&token_ctx, &env_ctx);
	}
	if (start != end)
		handle_word(&token_ctx, &env_ctx);
	token = create_token(NULL, TOKEN_END);
	add_token(&head, &current, token);
	return (head);
}
