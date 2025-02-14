/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/14 14:19:58 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


char	*add_shlvl(char *value)
{
	int num;
	char * new;
	//atoi, add 1, itoa, return
	num = ft_atoi(value);
	num = num + 1;

	new = ft_itoa(num); //MALLOC
	free(value);
	value = new;
	return (value);
}


void	set_key_value(char *env_str,t_env *new_node) // = in str
{
	char	*key;
	char	*value;
	char	*trimmed;
	int		index_first;

	//split by =
	index_first = 0;
	while (env_str[index_first] != '=')
		index_first++;
	key = ft_substr(env_str, 0, index_first); //MALLOC
	//if(!key)
		//free and exit
	value = ft_substr(env_str, index_first + 1, ft_strlen(env_str) - 1 - index_first);
	//printf("value: %s\n", value);
	//if(!value)
		//free and exit
//for value trim " " if there
	if (value[0] == '"' && value[ft_strlen(value)] == '"')
	{
		trimmed = ft_strtrim(value, "\"");
		free(value);
		value = trimmed;
	}
	//set in new node
	new_node->key = key;
	new_node->value = value;
}

void	add_env_back(t_env **environ, t_env *new_node)//add new node to end of list
{
	t_env	*last;
	//go to end of list
	if (*environ == NULL)
	{
		*environ = new_node;
		return ;
	}

	last = *environ;
	while (last->next)
		last = last->next;
	//set last node ->next -> new node
	last->next = new_node;
}


t_env	*set_env(char *envp[]) //create linked list w all env vars, increasing shlvl by 1
{
	printf("creating environ\n");
	t_env	*environ;
	t_env	*new_node;
	char	*equal;
	int		i;

	environ = NULL;
	new_node = NULL;
	i = 0;
	//loop thru envp
	while (envp[i])
	{//malloc space for node
		new_node = (t_env *)malloc(sizeof(t_env) * 1);
		//if (!new_node)
			//free and exit
	//check if = in str
		equal = ft_strchr(envp[i], '=');
		if (!equal)//no = in envp str
		{//assign str as key
			new_node->key = envp[i];
			new_node->value = NULL;
		}
		else //equal in str
			set_key_value(envp[i], new_node);
		new_node->next = NULL;
		//if shlvl add 1
		if (ft_strncmp(new_node->key, "SHLVL", 5) == 0)
		{	printf("adding to shlvl\n");

			//get value,  atoi, +1, itoa
			new_node->value = add_shlvl(new_node->value);
		}
		//connect to list
		add_env_back(&environ, new_node);

		equal = NULL;
		new_node = NULL;
		i++;
	}
	return (environ);
	
}

int	main (int argc, char *argv[], char *envp[]) //(void)
{
	char	*input;
	t_env	*environ;

	//TO DO: init protection ( like if !envp etc) 

	//1. load config files, init etc
	print_start();

	/*TO DO: set up env (SHLVL increases from std.!)
	*/
	environ = set_env(envp);
	//print_env(environ);

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
			//1. lexer: create tokens
			//(2. parser: takes tokens (and builds commmand list))

		//4. execute
		execute(environ);
			//creates processes, 
			//handles redirections/pipes,
			//decides if cmd is builtin or not etc and executes them
			//special cases: $?, 
		
		//5. free everything needed
		free(input);
		input = NULL;
		print_env(environ);

		printf("argc = %i\n", argc);
		printf("argv = %s\n", argv[0]);

	}
	//6 shutdown shell (also after signal)
	//rl_clear_history();	
	
	return (0);
}