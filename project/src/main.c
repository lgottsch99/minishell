/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/20 20:17:22 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t g_signal_status = 0;

int	main (int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	char	*input;
	t_env	*environ;
	int		exit_stat;
	Token		*tokens;
	t_command	*commands;
	char		**env_array;

	exit_stat = 0;


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
			write(STDOUT_FILENO, "exit\n", 5);//??needed
			free(input);
			free_env_list(&environ);
			rl_clear_history();
            exit(1); //free before  rl history etc  TODO
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
		//printf("you typed: %s\n", input);
		
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
		tokens = tokenize(input, exit_stat, env_array);
		free_2d_char(env_array);
		free(input);
		input = NULL;
		
		if (tokens)
		{

			print_tokens(tokens);	
			commands = parse_tokens(tokens); //heredoc now in parse_tokens
			print_commands(commands);
			//go thru cmd list and check for each cmd: 
			// t_command *current_command;
			// current_command = commands;
			free_tokens(tokens);
			tokens = NULL;	
		}
		
		// parsing end ------------------------
		printf("finished parsing\n");
		
		//4. execute
		execute(environ, &exit_stat, commands);

		//5. free everything needed TODO
		if (commands)
		{
			free_cmd_list(&commands);
			commands = NULL;
		}
	}
}