/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/01/28 16:32:32 by Watanudon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/*
	???
	somehow somewhere handling signals (ctrl C, ctrl D and ctrl \)
	how to $?

	
*/

int	main (void)//(int argc, char *argv[], char *envp[])
{
	char *input;

	//1. load config files, init etc
	print_start();

	//2. main loop
	while (1)
	{
		input = readline("mini_shell$ ");
		if (!input)
			break;
		//adding input to history
		add_history(input);
		printf("you typed: %s\n", input);

		//3. parse (and create AST), 
			//0. handle special quotes ('' ""), heredoc (<<)
				//handle word expansions, env vars (eg. $USER)
			//1. lexer: create tokens
			//(2. parser: takes tokens (and builds AST))

		//4. execute (AST)
			//creates processes, 
			//handles redirections/pipes,
			//decides if cmd is builtin or not etc and executes them
			//special cases: $?, 
		
		//5. free everything needed
		free(input);
	}
	//6 shutdown shell (also after signal)
	//rl_clear_history();	

	return (0);
}