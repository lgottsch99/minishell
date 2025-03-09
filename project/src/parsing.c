/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:02:40 by dvasilen          #+#    #+#             */
/*   Updated: 2025/03/08 16:57:32 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_argument(t_command *command, char *arg)
{
	int		count;
	char	**new_args;

	count = 0;
	if (command->args)
	{
		while (command->args[count])
			count++;
	}
	new_args = realloc(command->args, (count + 2) * sizeof(char *));
	if (!new_args)
		return ;
	command->args = new_args;
	command->args[count] = arg;
	command->args[count + 1] = NULL;
}

int	is_builtin(char *arg)
{
	if (strcmp(arg, "echo") == 0 || strcmp(arg, "cd") == 0
		|| strcmp(arg, "pwd") == 0 || strcmp(arg, "export") == 0
		|| strcmp(arg, "unset") == 0 || strcmp(arg, "env") == 0
		|| strcmp(arg, "exit") == 0)
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

t_command	*parse_tokens(Token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*command;
	char		*arg;

	head = NULL;
	current = NULL;
	command = create_command();
	if (!command)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == TOKEN_REDIRECT_HEREDOC)
		{
			command->heredoc_delimetr = ft_strdup(tokens->next->value);
			if (!command->heredoc_delimetr)
			{
				free_commands(head);
				return (NULL);
			}
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_WORD)
		{
			arg = ft_strdup(tokens->value);
			if (!arg)
			{
				free_commands(head);
				return (NULL);
			}
			add_argument(command, arg);
			if (command->args[0] && is_builtin(command->args[0]))
				command->is_builtin = 1;
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			if (!head)
				head = command;
			if (current)
				current->next = command;
			current = command;
			command = create_command();
			if (!command)
			{
				free_commands(head);
				return (NULL);
			}
		}
		else if (tokens->type == TOKEN_REDIRECT_IN)
		{
			if (tokens->next && tokens->next->type == TOKEN_WORD)
			{
				command->input_file = ft_strdup(tokens->next->value);
				if (!command->input_file)
				{
					free_commands(head);
					return (NULL);
				}
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_REDIRECT_OUT)
		{
			if (tokens->next && tokens->next->type == TOKEN_WORD)
			{
				command->output_file = ft_strdup(tokens->next->value);
				if (!command->output_file)
				{
					free_commands(head);
					return (NULL);
				}
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_REDIRECT_APPEND)
		{
			if (tokens->next && tokens->next->type == TOKEN_WORD)
			{
				command->append_mode = 1;
				command->output_file = ft_strdup(tokens->next->value);
				if (!command->output_file)
				{
					free_commands(head);
					return (NULL);
				}
				tokens = tokens->next;
			}
		}
		tokens = tokens->next;
	}
	if (!head)
		head = command;
	if (current)
		current->next = command;
	return (head);
}

void	print_commands(t_command *commands)
{
	while (commands)
	{
		printf("t_command:\n");
		for (int i = 0; commands->args[i]; i++)	
			printf("  Arg %d: %s\n", i, commands->args[i]);
		//	if (commands->input_file)
		printf("  Input file: %s\n", commands->input_file);
		//if (commands->output_file)
		printf("  Output file: %s\n", commands->output_file);
		//if (commands->heredoc_delimetr)
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

void	free_commands(t_command *commands)
{
	t_command	*tmp;
	int			i;

	while (commands)
	{
		tmp = commands;
		commands = commands->next;
		if (tmp->args)
		{
			i = 0;
			while (tmp->args[i])
			{
				free(tmp->args[i]);
				i++;
			}
			free(tmp->args);
		}
		if (tmp->input_file)
			free(tmp->input_file);
		if (tmp->output_file)
			free(tmp->output_file);
		if (tmp->heredoc_delimetr)
			free(tmp->heredoc_delimetr);
		if (tmp->heredoc_file)
			free(tmp->heredoc_file);
		if (tmp->exec_path)
			free(tmp->exec_path);
		free(tmp);
	}
}
