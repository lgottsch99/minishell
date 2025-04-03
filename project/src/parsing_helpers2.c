/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-26 15:03:52 by dvasilen          #+#    #+#             */
/*   Updated: 2025-03-26 15:03:52 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd->input_file);
	free(cmd->output_file);
	free(cmd->heredoc_delimetr);
	free(cmd->heredoc_file);
	free(cmd->exec_path);
	free(cmd);
}

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands)
	{
		tmp = commands;
		commands = commands->next;
		free_command(tmp);
	}
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*cmd;
	int			count;
	t_parse_ctx	ctx;

	head = NULL;
	current = NULL;
	count = 1;
	cmd = create_command();
	ctx = (t_parse_ctx)
	{
		&cmd, &head, &current, &count
	};
	if (!cmd)
		return (NULL);
	while (tokens && process_token(&tokens, &ctx))
		tokens = tokens->next;
	if (!head)
		head = cmd;
	if (current)
		current->next = cmd;
	return (head);
}
