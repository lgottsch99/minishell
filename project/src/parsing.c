/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:02:40 by dvasilen          #+#    #+#             */
/*   Updated: 2025/02/28 14:22:14 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_argument(t_command *command, char *arg)
{
	int	count;
	
	count = 0;
	if (command->args)
	{
		while(command->args[count])
			count++;
	}
	command->args = realloc(command->args, (count + 2) * sizeof(char *)); //TODO check ft if allowed
	if (!command->args)
		return;
	command->args[count] = arg;
	command->args[count + 1] = NULL;
}

int	is_builtin(char *arg)
{
	if (strcmp(arg, "echo") == 0 || strcmp(arg, "cd") == 0 || strcmp(arg, "pwd") == 0 
			|| strcmp(arg, "export") == 0 || strcmp(arg, "unset") == 0 
				|| strcmp(arg, "env") == 0 || strcmp(arg, "exit") == 0)
		return (1);
	return(0);
}

t_command	*create_command()
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->next = NULL;
	cmd->is_builtin = 0;
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
	while(tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			arg = ft_strdup(tokens->value);
			add_argument(command, arg);
			if (command->args[0] && is_builtin(command->args[0])) 
				command->is_builtin = 1;
		}		
		else if(tokens->type == TOKEN_PIPE)
		{
			if (!head)
				head = command;
			if (current)
				current->next = command;
			current = command;
			command = create_command();
		}
		else if(tokens->type == TOKEN_REDIRECT_IN)
		{
			if (tokens->next && tokens->next->type == TOKEN_WORD)
			{
				command->input_file = ft_strdup(tokens->next->value);
 				tokens = tokens->next;
			}
		}
		else if(tokens->type == TOKEN_REDIRECT_OUT)
		{
			if (tokens->next && tokens->next->type == TOKEN_WORD)
			{
				command->output_file = ft_strdup(tokens->next->value);
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

void	print_commands(t_command *commands) {
	while (commands)
	{
		printf("t_command:\n");
		//printf("command: %s\n", commands->command);

		for (int i = 0; commands->args[i]; i++)	
			printf("  Arg %d: %s\n", i, commands->args[i]);
		//if (commands->input_file)
		printf("  Input file: %s\n", commands->input_file);
		//if (commands->output_file)
		printf("  Output file: %s\n", commands->output_file);
		if (commands->append_mode)
			printf("  Append output: yes\n");
		if (commands->is_builtin)
			printf("  Builtin: yes\n");
		if (commands->exec_path)
			printf("  exec path: %s\n", commands->exec_path);
		commands = commands->next;
	}
}

void	free_commands(t_command *commands)
{
	t_command	*tmp;
	while (commands)
	{
		tmp = commands;
		commands = commands->next;
		if (tmp->args){
			for (int i = 0; tmp->args[i]; i++)
				free(tmp->args[i]);
			free(tmp->args);
		}
		if (tmp->input_file)
			free(tmp->input_file);
		if (tmp->output_file)
			free(tmp->output_file);
		free(tmp);
	}
}