/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-26 14:42:54 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-26 14:42:54 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_heredoc_parsing(t_command **cmd, Token **tokens,
		int count, t_command **head)
{
	if ((*cmd)->heredoc_file)
	{
		free((*cmd)->heredoc_delimetr);
		(*cmd)->heredoc_delimetr = NULL;
		remove_heredoc(&(*cmd)->heredoc_file);
	}
	(*cmd)->heredoc_delimetr = ft_strdup((*tokens)->next->value);
	if (!(*cmd)->heredoc_delimetr)
		return (1);
	(*cmd)->heredoc_file = read_heredoc((*cmd)->heredoc_delimetr, count);
	if (!(*cmd)->heredoc_file)
	{
		free_commands(*head);
		free_command(*cmd);
		return (0);
	}
	*tokens = (*tokens)->next;
	return (1);
}

int	handle_word_parsing(t_command *cmd, char *value, t_command **head)
{
	char	*arg;

	arg = ft_strdup(value);
	if (!arg)
	{
		free_commands(*head);
		free_command(cmd);
		return (0);
	}
	add_argument(cmd, arg);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
		cmd->is_builtin = 1;
	return (1);
}

int	handle_redir(t_command **cmd, Token **tokens, t_command **head, int mode)
{
	char	**target;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
		return (1);
	if (mode == 1)
	{
		if ((*tokens)->type == TOKEN_REDIRECT_APPEND)
			(*cmd)->append_mode = 1;
		target = &(*cmd)->output_file;
	}
	else
		target = &(*cmd)->input_file;
	*target = ft_strdup((*tokens)->next->value);
	if (!*target)
	{
		free_commands(*head);
		free_command(*cmd);
		return (0);
	}
	*tokens = (*tokens)->next;
	return (1);
}

void	handle_pipe_parsing(t_command **head,
	t_command **current, t_command **cmd, int *count)
{
	if (!*head)
		*head = *cmd;
	if (*current)
		(*current)->next = *cmd;
	*current = *cmd;
	*cmd = create_command();
	if (!*cmd)
		return ;
	if ((*cmd)->heredoc_file)
		(*count)++;
}
