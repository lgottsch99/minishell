/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/26 16:02:18 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	main (int argc, char *argv[], char *envp[]) //(void)
{
	(void)argc; //marking unused
	(void)argv;
	char	*input;
	t_env	*environ;
	int		exit_stat;

	exit_stat = 0;
	//TO DO: init protection ( like if !envp etc) 

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
		input = readline("mini_shell$ ");
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
			//0. handle special quotes ('' ""), heredoc (<<)
			//1. lexer: create tokens
			//(2. parser: takes tokens (and builds commmand list))

		//4. execute
		execute(environ, &exit_stat);
			//creates processes, 
			//handles redirections/pipes,
			//decides if cmd is builtin or not etc and executes them
			//special cases: $?, 
		
		//5. free everything needed TODO
		free(input);
		input = NULL;

	}
	//6 shutdown shell (also after signal)
	free_env_list(&environ);
	//rl_clear_history();	
	
	return (exit_stat);
}