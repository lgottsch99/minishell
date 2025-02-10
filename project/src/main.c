/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:16:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/10 15:27:18 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/*
	???
	somehow somewhere handling signals (ctrl C, ctrl D and ctrl \)
	how to $?
	 where to handle word expansions, env vars (eg. $USER)

	
*/

char	*add_shlvl(char *var)
{

				printf("creating new shlvl\n");

	char	*value;
	char	*new_str;
	char	*tmp;
	int		num;
	int 	len;
	int		i;

	new_str = NULL;
	len = ft_strlen(var);
	//get value,  atoi, +1, itoa
	value = ft_substr(var, 6, (len - 6)); //MALLOC
		//if (!value)
	printf("current value shlvl is: %s\n", value);
	
	num = ft_atoi(value);
	num += 1;
	free(value);
	value = ft_itoa(num); //MALLOC
	printf("new value shlvl is: %s\n", value);

	//create new str for shlvl
	len = 0;
	len = ft_strlen(value) + 7; //shlvl=  + \0 = 7
	new_str = (char *)malloc(sizeof(char) * len); //MALLOC
		//if (!new_str)
	//copy first part, move pointer by 6, copy 2nd part
	ft_strlcpy(new_str, "SHLVL=", 7);

	tmp = new_str;
	i = 0;
	while (i < 6)
	{
		tmp++;
		i++;
	}
	ft_strlcpy(tmp, value, len);
	free(var);
	free(value);
		printf("final new value shlvl is: %s\n", new_str);

	return(new_str);

}


t_list	*set_env(char *envp[]) //create linked list w all env vars, increasing shlvl by 1
{
	printf("creating environ\n");
	t_list	*environ;
	t_list	*new_node;
	char	*var;
	int		i;

	environ = NULL;
	new_node = NULL;
	var = NULL;
	i = 0;
	//loop thru envp
	while (envp[i])
	{
		//malloc space for string
		var = (char *)malloc(sizeof(char) * (ft_strlen(envp[i]) + 1));
		if (!var)
		{
			//free list before
			exit(1007);
		}
		//copy value into space
		ft_strlcpy(var, envp[i], (ft_strlen(envp[i]) + 1));
		
		//if shlvl increase by 1
		if (ft_strncmp(var, "SHLVL", 5) == 0)
		{
			printf("adding to shlvl\n");
			//get value,  atoi, +1, itoa
			var = add_shlvl(var);
			printf("finito adding to shlvl\n");
		}
		//for each create new node
		new_node = ft_lstnew(var);
		//connect to list
		ft_lstadd_back(&environ, new_node);
		i++;
	}
	
	return(environ);
}

int	main (int argc, char *argv[], char *envp[]) //(void)
{
	char	*input;
	t_list	*environ;

	//TO DO: init protection ( like if !envp etc) 

	//1. load config files, init etc
	print_start();

	/*TO DO: set up env (SHLVL increases from std.!)
	*/
	environ = set_env(envp);
	print_env(environ);

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
		printf("argc = %i\n", argc);
		printf("argv = %s\n", argv[0]);

	}
	//6 shutdown shell (also after signal)
	//rl_clear_history();	
	
	return (0);
}