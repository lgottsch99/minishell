/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/01/26 18:14:03 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/*
	???
	somehow somewhere handling signals,
	
*/

int	main (void)
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
		// else //adding input to history
		// add_history
		printf("you typed: %s\n", input);

		//3. parse (and create AST), 
			//0. handle special quotes ('' "")
			//1. lexer: create tokens
			//handle word expansions (eg. $USER)
			//2. parser: takes tokens and builds AST

		//4. execute AST
			//creates processes, 
			//handles redirections/pipes,
			//decides if cmd is builtin or not etc and executes them
			//special cases: $?, 
			
		//5. free everything needed
		free(input);
	}
	return (0);
}