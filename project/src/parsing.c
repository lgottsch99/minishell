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

void	print_commands(t_command *commands) { //remove whole ft later
	while (commands)
	{
		printf("t_command:\n");
		if (commands && commands->args)
		{	for (int i = 0; commands->args[i]; i++)	
				printf("  Arg %d: %s\n", i, commands->args[i]);
		}
		printf("  Input file: %s\n", commands->input_file);
		printf("  Output file: %s\n", commands->output_file);
		printf("Heredoc del.: %s\n", commands->heredoc_delimetr);
		if (commands->heredoc_file)
			printf("Heredoc filename: %s\n", commands->heredoc_file);
		if (commands->append_mode)
			printf("  Append output: yes\n");
		if (commands->is_builtin)
			printf("  Builtin: yes\n");
		if (commands->exec_path)
			printf("  exec path: %s\n", commands->exec_path);
		commands = commands->next;
		printf("\n");
	}
}

t_command	*parse_tokens(Token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*cmd;
	int			count;

	head = NULL;
	current = NULL;
	count = 1;
	cmd = create_command();
	if (!cmd)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == TOKEN_REDIRECT_HEREDOC)
		{
			if (!handle_heredoc_parsing(&cmd, &tokens, count, &head))
				return (NULL);
		}
		else if (tokens->type == TOKEN_WORD)
		{
			if (!handle_word_parsing(cmd, tokens->value, &head))
				return (NULL);
		}
		else if (tokens->type == TOKEN_PIPE)
			handle_pipe_parsing(&head, &current, &cmd, &count);
		else if (tokens->type == TOKEN_REDIRECT_IN)
		{
			if (!handle_redir(&cmd, &tokens, &head, 0))
				return (NULL);
		}
		else if (tokens->type >= TOKEN_REDIRECT_OUT)
		{
			if (!handle_redir(&cmd, &tokens, &head, 1))
				return (NULL);
		}
		tokens = tokens->next;
	}
	if (!head)
		head = cmd;
	if (current)
		current->next = cmd;
	return (head);
}
