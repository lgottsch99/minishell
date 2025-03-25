/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/25 16:20:52 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t g_signal_status = 0;


int	handle_input(char *input, t_env	*environ, int *exit_stat)
{
	g_signal_status = 0;
	if (!input)  //ctrl-D handling, its not a signal but a eof detecter
	{
		write(STDOUT_FILENO, "exit\n", 5);
		free(input);
		free_env_list(&environ);
		rl_clear_history();
		exit(*exit_stat);
	}
	if (g_signal_status == SIGINT)
		return (1);
	if (input[0] == '\0') //just typing enter at prompt
		return (1);
	add_history(input);		
	return (0);
}

t_command	*parsing(char **input, t_env *environ, int *exit_stat)
{
	char		**env_array;
	Token		*tokens;
	t_command	*commands;

	commands = NULL;
	env_array = env_to_array(environ);
	if (!env_array)
		return(NULL);
	tokens = tokenize(*input, *exit_stat, env_array);
	free_2d_char(env_array);
	free(*input);
	*input = NULL;
	if (tokens)
	{
		print_tokens(tokens);//rm
		commands = parse_tokens(tokens);
		free_tokens(tokens);
		tokens = NULL;
		if (!commands) //return null in case of heredoc ctrl c
			return(NULL);
		print_commands(commands);//rm
	}
	return (commands);
}

int	main_loop(int *exit_stat, t_env *environ)
{
	char 	*input;
	t_command	*commands;
	
	input = readline("***miniShell***$ ");
	if (handle_input(input, environ, exit_stat) == 1)
	{
		free(input);
		return (1) ;
	}
	//3. parse
	commands = parsing(&input, environ, exit_stat); //ret NULL in case of error
	if (!commands)
	{
		free(input);
		return (1);
	}
	//4. execute
	execute(environ, exit_stat, commands);	
	//5. free everything needed TODO
	if (commands)
	{
		free_cmd_list(&commands);
		commands = NULL;
	}
	return (0);
}

int	main (int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	// char	*input;
	t_env	*environ;
	int		exit_stat;
	// t_command	*commands;

	exit_stat = 0;
	if (init_shell(&environ, envp) == 1)
		return (1);
	while (1)//2. main loop
	{
		if (main_loop(&exit_stat, environ) == 1)
			continue ;
		// input = readline("***miniShell***$ ");
		// if (handle_input(input, environ, &exit_stat) == 1)
		// {
		// 	free(input);
		// 	continue ;
		// }
		// //3. parse
		// commands = parsing(&input, environ, &exit_stat); //ret NULL in case of error
		// if (!commands)
		// {
		// 	free(input);
		// 	continue ;
		// }
		// //4. execute
		// execute(environ, &exit_stat, commands);	
		// //5. free everything needed TODO
		// if (commands)
		// {
		// 	free_cmd_list(&commands);
		// 	commands = NULL;
		// }
	}
}

// int	main (int argc, char *argv[], char *envp[])
// {
// 	(void)argc;
// 	(void)argv;
// 	char	*input;
// 	t_env	*environ;
// 	int		exit_stat;
// 	// Token		*tokens;
// 	t_command	*commands;
// 	// char		**env_array;

// 	exit_stat = 0;
// 	if (init_shell(&environ, envp) == 1)
// 		return (1);

// 	//2. main loop
// 	while (1)
// 	{
// 		input = readline("***miniShell***$ ");
// 		if (handle_input(input, environ, &exit_stat) == 1)
// 		{
// 			free(input);
// 			continue ;

// 		}
// 		// g_signal_status = 0;
// 		// if (!input)  //ctrl-D handling, its not a signal but a eof detecter
// 		// {
// 		// 	write(STDOUT_FILENO, "exit\n", 5);
// 		// 	free(input);
// 		// 	free_env_list(&environ);
// 		// 	rl_clear_history();
// 		// 	exit(exit_stat);
// 		// }
// 		// if (g_signal_status == SIGINT)
// 		// {
// 		// 	free(input);
// 		// 	continue;
// 		// }
// 		// if (input[0] == '\0') //just typing enter at prompt
// 		// {
// 		// 	free(input);
// 		// 	continue;
// 		// }
// 		// add_history(input);		

// 		//3. parse ------------------------

// 		commands = parsing(&input, environ, &exit_stat); //ret NULL in case of error
// 		if (!commands)
// 		{
// 			free(input);
// 			continue ;
// 		}

// 		// env_array = env_to_array(environ);
// 		// if (!env_array)
// 		// {
// 		// 	free(input);
// 		// 	perror("mallocing env array: ");
// 		// 	exit_stat = 1;
// 		// 	continue ;
// 		// }
// 		// tokens = tokenize(input, exit_stat, env_array);
// 		// free_2d_char(env_array);
// 		// free(input);
// 		// input = NULL;
		
// 		// if (tokens)
// 		// {

// 		// 	print_tokens(tokens);	
// 		// 	commands = parse_tokens(tokens);
// 		// 	free_tokens(tokens);
// 		// 	tokens = NULL;
// 		// 	if (!commands) //return null in case of heredoc ctrl c
// 		// 	{
// 		// 		// printf("heredoc interrupted\n");
// 		// 		free_tokens(tokens);
// 		// 		free(input);
// 		// 		continue;
// 		// 	}
// 		// 	print_commands(commands);	
// 		// }
// 		// //printf("finished parsing\n");
		
// 		//4. execute
// 		execute(environ, &exit_stat, commands);
		
// 		//5. free everything needed TODO
// 		if (commands)
// 		{
// 			free_cmd_list(&commands);
// 			commands = NULL;
// 		}
// 	}
// }