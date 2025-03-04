/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/27 18:22:30 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	main (int argc, char *argv[], char *envp[])
{
	(void)argc; //marking unused
	(void)argv;
	char	*input;
	t_env	*environ;
	int		exit_stat;
	Token		*tokens;
	t_command	*commands;

	exit_stat = 0;
	// init protection ( like if !envp etc)
	if (!envp)
	{
		printf("Failed to receive Environment from OS\n");
		return (1);
	}

	//1. load config files, init etc
	print_start();

	//set up env (SHLVL increases from std.!)
	environ = set_env(envp); //MALLOC
	if (!environ)
	{
		printf("Failed to set up Environment\n");
		return (1);
	}
	//print_env(environ);

	//2. main loop
	while (1)
	{
		input = readline("***miniShell***$ ");
		if (!input)
		{
			printf("readline error\n");
			exit_stat = 1;
			break;
		}
		//adding input to history
		add_history(input);
		printf("you typed: %s\n", input);

		//3. parse (and create AST), 
		tokens = tokenize(input, exit_stat, envp); //TO DO change envp to own environ
		if (tokens)
		{
			print_tokens(tokens);	
			commands = parse_tokens(tokens);
			if (commands)
			{
				if(commands->heredoc_delimetr)
					commands->heredoc_input = read_heredoc(commands->heredoc_delimetr);
			}
			free_tokens(tokens);
			tokens = NULL;	
		}
		//print_commands(commands);
			//0. handle special quotes ('' ""), heredoc (<<)
			//1. lexer: create tokens
			//(2. parser: takes tokens (and builds commmand list))

		//4. execute
		execute(environ, &exit_stat, commands);
			//creates processes, 
			//handles redirections/pipes,
			//decides if cmd is builtin or not etc and executes them
			//special cases: $?, 
		clean_heredoc(commands); //todo

		//5. free everything needed TODO
		free(input);
		input = NULL;

		free_commands(commands);
		commands = NULL;

	}
	//6 shutdown shell (also after signal)
	free_env_list(&environ);
	//rl_clear_history();	
	
	return (exit_stat);
}