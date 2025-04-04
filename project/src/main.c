/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 13:51:42 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

int	handle_input(char *input, t_env	*environ, int *exit_stat)
{
	g_signal_status = 0;
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		free(input);
		free_env_list(&environ);
		rl_clear_history();
		exit(*exit_stat);
	}
	if (g_signal_status == SIGINT)
		return (1);
	if (input[0] == '\0')
		return (1);
	add_history(input);
	return (0);
}

t_command	*parsing(char **input, t_env *environ, int *exit_stat)
{
	char		**env_array;
	t_token		*tokens;
	t_command	*commands;

	commands = NULL;
	env_array = env_to_array(environ);
	if (!env_array)
		return (NULL);
	tokens = tokenize(*input, *exit_stat, env_array);
	free_2d_char(env_array);
	free(*input);
	*input = NULL;
	if (tokens)
	{
		commands = parse_tokens(tokens);
		free_tokens(tokens);
		tokens = NULL;
		if (!commands)
			return (NULL);
	}
	return (commands);
}

int	main_loop(int *exit_stat, t_env *environ)
{
	char		*input;
	t_command	*commands;

	input = readline("***miniShell***$ ");
	if (handle_input(input, environ, exit_stat) == 1)
	{
		free(input);
		return (1);
	}
	commands = parsing(&input, environ, exit_stat);
	if (!commands)
	{
		free(input);
		return (1);
	}
	execute(environ, exit_stat, commands);
	if (commands)
	{
		free_cmd_list(&commands);
		commands = NULL;
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_env	*environ;
	int		exit_stat;

	(void)argc;
	(void)argv;
	exit_stat = 0;
	if (init_shell(&environ, envp) == 1)
		return (1);
	while (1)
	{
		if (main_loop(&exit_stat, environ) == 1)
			continue ;
	}
}

//OG MAIN NOT NORMED:

// int	main (int argc, char *argv[], char *envp[])
// {
// 	(void)argc;
// 	(void)argv;
// 	char	*input;
// 	t_env	*environ;
// 	int		exit_stat;
// 	// t_token		*tokens;
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
// 		// if (!input)  //ctrl-D handling, 
// its not a signal but a eof detecter
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
//ret NULL in case of error
// 		commands = parsing(&input, environ, &exit_stat); 
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

// void	print_commands(t_command *commands) { //remove whole ft later
// 	while (commands)
// 	{ //////rm
// 		printf("t_command:\n");
// 		if (commands && commands->args)
// 		{	for (int i = 0; commands->args[i]; i++)	
// 				printf("  Arg %d: %s\n", i, commands->args[i]);
// 		}
// 		printf("  Input file: %s\n", commands->input_file);
// 		printf("  Output file: %s\n", commands->output_file);
// 		printf("Heredoc del.: %s\n", commands->heredoc_delimetr);
// 		if (commands->heredoc_file)
// 			printf("Heredoc filename: %s\n", commands->heredoc_file);
// 		if (commands->append_mode)
// 			printf("  Append output: yes\n");
// 		if (commands->is_builtin)
// 			printf("  Builtin: yes\n");
// 		if (commands->exec_path)
// 			printf("  exec path: %s\n", commands->exec_path);
// 		commands = commands->next;
// 		printf("\n");
// 	}
// }