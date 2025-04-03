/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:02:40 by dvasilen          #+#    #+#             */
/*   Updated: 2025/03/25 17:18:15 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_argument(t_command *command, char *arg)
{
	int		count;
	int		i;
	char	**new_args;

	count = 0;
	if (command->args)
		while (command->args[count])
			count++;
	new_args = (char **)malloc((count + 2) * sizeof(char *));
	if (!new_args)
		return ;
	if (command->args)
	{
		i = -1;
		while (++i < count)
			new_args[i] = command->args[i];
		free(command->args);
	}
	command->args = new_args;
	command->args[count] = arg;
	command->args[count + 1] = NULL;
}

int	is_builtin(char *arg)
{
	if (ft_strcmp(arg, "echo") == 0 || ft_strcmp(arg, "cd") == 0
		|| ft_strcmp(arg, "pwd") == 0 || ft_strcmp(arg, "export") == 0
		|| ft_strcmp(arg, "unset") == 0 || ft_strcmp(arg, "env") == 0
		|| ft_strcmp(arg, "exit") == 0)
		return (1);
	return (0);
}

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->next = NULL;
	cmd->is_builtin = 0;
	cmd->heredoc_file = NULL;
	cmd->heredoc_delimetr = NULL;
	cmd->exec_path = NULL;
	return (cmd);
}

int	handle_redirection_parsing(t_token **token, t_parse_ctx *ctx, int is_output)
{
	t_token	*next_token;

	next_token = (*token)->next;
	if (!next_token || next_token->type != TOKEN_WORD)
		return (0);
	if (!handle_redir(ctx->cmd, token, ctx->head, is_output))
		return (0);
	*token = next_token;
	return (1);
}

int	handle_heredoc_token(t_token **token, t_parse_ctx *ctx)
{
	if (!handle_heredoc_parsing(ctx->cmd, token, *(ctx->count), ctx->head))
		return (0);
	return (1);
}
