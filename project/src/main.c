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

volatile sig_atomic_t g_signal_status = 0;

int	main (int argc, char *argv[], char *envp[])
{
	(void)argc; //marking unused
	(void)argv;
	char	*input;
	t_env	*environ;
	int		exit_stat;
	Token		*tokens;
	t_command	*commands;
	char		**env_array;

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

	setup_signals();

	//2. main loop
	while (1)
	{
		input = readline("***miniShell***$ ");
		if (!input)  //ctrl-D handling, its not a signal but a eof detecter
		{
			exit_stat = 1;
			write(STDOUT_FILENO, "exit\n", 5);
            exit(EXIT_SUCCESS);
		}
		if (g_signal_status == SIGINT)
		{
			g_signal_status = 0;
			continue;
		}
		//adding input to history
		add_history(input);
		printf("you typed: %s\n", input);

		//3. parse (and create AST), 
		env_array = env_to_array(environ);
		if (!env_array)
		{
			free(input);
			free_env_list(&environ);
			return (1);
		}
		tokens = tokenize(input, exit_stat, env_array); //TO DO change envp to own environ
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
		
		while (*env_array)
		{
			free(*env_array);
			env_array++;
		}

		execute(environ, &exit_stat, commands);
			//creates processes, 
			//handles redirections/pipes,
			//decides if cmd is builtin or not etc and executes them
			//special cases: $?, 
		clean_heredoc(commands); 

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