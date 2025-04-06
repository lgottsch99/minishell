/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenising.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:02:32 by dvasilen          #+#    #+#             */
/*   Updated: 2025/04/06 14:18:02 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	data_init(t_tdata *data, char *input)
{
	data->head = NULL;
	data->current = NULL;
	data->token = NULL;
	data->start = input;
	data->end = input;
	data->token_ctx = (t_tokenizeContext){&data->start,
		&data->end, &data->head, &data->current};
	while (ft_isspace(*data->start))
		data->start++;
	data->end = data->start;
	if (!*(data->start))
	{
		data->token = create_token(NULL, TOKEN_END);
		add_token(&data->head, &data->current, data->token);
		return (0);
	}
	return (1);
}

void	handle_quotes(t_tdata *data, int last_exit_status, char **envp)
{
	if (data->end > data->start)
		handle_word(&data->token_ctx, &data->env_ctx);
	if (*data->end == '"')
	{
		data->quote_ctx = (t_quote_context)
		{
			.start = &data->start,
			.end = &data->end,
			.head = &data->head,
			.current = &data->current,
			.last_exit_status = last_exit_status,
			.envp = envp
		};
		handle_double_quote(&data->quote_ctx);
	}
	else
		handle_single_quote(&data->start, &data->end,
			&data->head, &data->current);
}

void	handle_env(t_tdata *data, int last_exit_status, char **envp)
{
	data->env_ctx = (t_envVarContext){&data->start, &data->end, &data->head,
		&data->current, last_exit_status, envp, 1};
	handle_env_var(&data->env_ctx);
	while (*data->end && !ft_isspace(*data->end) && *data->end != '|'
		&& *data->end != '>' && *data->end != '<' && *data->end != '=')
		(*data->end)++;
	data->start = data->end;
}

static void	handle_redirections(t_tdata *data)
{
	if (*data->end == '>')
		handle_redir_more(&data->start, &data->end,
			&data->head, &data->current);
	else if (*data->end == '<')
		handle_redir_less(&data->start, &data->end,
			&data->head, &data->current);
}

t_token	*tokenize(char *input, int last_exit_status, char **envp)
{
	t_tdata	data;

	if (!data_init(&data, input))
		return (NULL);
	while (*(data.end))
	{
		if (ft_isspace(*data.end))
			handle_space(&data);
		else if (*data.end == '|')
			handle_pipe(&data.start, &data.end, &data.head, &data.current);
		else if (*data.end == '>' || *data.end == '<')
			handle_redirections(&data);
		else if (*data.end == '"' || *data.end == '\'')
			handle_quotes(&data, last_exit_status, envp);
		else if (*data.end == '$')
			handle_env(&data, last_exit_status, envp);
		else
			handle_word(&data.token_ctx, &data.env_ctx);
	}
	if (data.start != data.end)
		handle_word(&data.token_ctx, &data.env_ctx);
	data.token = create_token(NULL, TOKEN_END);
	add_token(&data.head, &data.current, data.token);
	return (data.head);
}
