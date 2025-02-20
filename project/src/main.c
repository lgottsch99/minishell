/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/16 17:18:23 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	main (int argc, char *argv[], char *envp[]) 
{
	char		*input;
	t_env		*environ;
	Token		*tokens;
	int			last_exit_status;
	t_command	*commands;

	
	print_start();

	environ = set_env(envp); 
	if (!environ) 
	{
        fprintf(stderr, "Failed to initialize environment.\n");
        return 1;
    }

	last_exit_status = 0;
	while (1)
	{
		input = readline("mini_shell$ ");
		if (!input){
			printf("quitting minishell\n");
			break;
		}
		add_history(input);
		printf("you typed: %s\n", input);

		tokens = tokenize(input, last_exit_status, envp);
		if (!tokens)
		{
			fprintf(stderr, "tokinization failed.\n");
			free(input);
			continue;
		}
		print_tokens(tokens);

		commands = parse_tokens(tokens);
		//last_exit_status = execute(environ); //todo
		execute(environ);
		free_tokens(tokens);
		free_commands(commands); 
		free(input);
		input = NULL;

		printf("argc = %i\n", argc);
		printf("argv = %s\n", argv[0]);
	}
	free_env_list(&environ);
	rl_clear_history();	
	
	
	return (0);
}