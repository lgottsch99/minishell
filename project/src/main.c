/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/15 17:41:21 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t g_signal_status = 0;


int init_shell(t_env **environ, char *envp[])
{
	print_start();
	//set up env (SHLVL increases from std.!)
	*environ = set_env(envp); //MALLOC
	if (!environ)
	{
		printf("Failed to set up Environment\n");
		return (1);
	}
	//print_env(environ);

	setup_signals();
	return (0);
}


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
	if (init_shell(&environ, envp) == 1)
		return (1);


	//2. main loop
	while (1)
	{
		input = readline("***miniShell***$ ");
		if (!input)  //ctrl-D handling, its not a signal but a eof detecter
		{
			exit_stat = 1;
			write(STDOUT_FILENO, "exit\n", 5);
			free(input);
			free_env_list(&environ);
            exit(EXIT_SUCCESS); //free before  rl history etc  TODO
		}
		if (g_signal_status == SIGINT)//ctrl c handling
		{
			g_signal_status = 0;
			free(input);
			continue;
		}
		if (input[0] == '\0') //just typing enter at prompt
		{
			free(input);
			continue;
		}
		//adding input to history
		add_history(input);
		printf("you typed: %s\n", input);
		
		//check if heredoc, if yes separate readline TODO 

		//3. parse ------------------------
		env_array = env_to_array(environ);
		if (!env_array)
		{
			free(input);
			free_env_list(&environ);
			rl_clear_history();
			return (1);
		}
		tokens = tokenize(input, exit_stat, env_array); //TO DO change envp to own environ
		//freeing everything not needed from now 
		free_2d_char(env_array);
		free(input);
		input = NULL;
		
		if (tokens)
		{
			print_tokens(tokens);	
			commands = parse_tokens(tokens);

			//go thru cmd list and check for each cmd: 
			t_command *current_command;
			int count;

			count = 0;
			current_command = commands;
			while (current_command) //handling here doc within pipeline
			{
				printf("checking heredoc\n");
				// Read the heredoc for each command that needs it
				if (current_command->heredoc_delimetr)
				{
					printf("found heredoc del\n");
					current_command->heredoc_file = read_heredoc(current_command->heredoc_delimetr, ++count);
				}
					
				current_command = current_command->next;
			}
			free_tokens(tokens);
			tokens = NULL;	
		}
		
		// parsing end ------------------------
		printf("finished parsing\n");
		
		//4. execute
		execute(environ, &exit_stat, commands);

		//5. free everything needed TODO
		// free(input);
		// input = NULL;

		if (commands)
		{
			free_cmd_list(&commands);
			commands = NULL;
		}

	}
	//6 shutdown shell (also after signal) DO WE EVER ENTER AFTER MAIN LOOP?
	// if (environ)
	// 	free_env_list(&environ);
	// if (commands)
	// {
	// 	free_cmd_list(&commands);
	// 	commands = NULL;
	// }
	// rl_clear_history();
	
	// return (exit_stat);
}