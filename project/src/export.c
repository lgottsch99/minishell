// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   export.c                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/02/11 18:44:34 by lgottsch          #+#    #+#             */
// /*   Updated: 2025/02/13 18:27:58 by lgottsch         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../includes/minishell.h"

// //SHELL VAR RULES:  A variable name could contain any alphabet (a-z, A-Z), any digits (0-9), and an underscore ( _ ). However, a variable name must start with an alphabet or underscore. It can never start with a number. 
int	check_shellvar_rules(t_command *cmd)
{//chck if all args valid (see rules below), else error, stop
	int	i;
	int	y;
	
	i = 0;
	y = 0;
	while (cmd->args[i])
	{
		//check char[0]: if not a-z or '_' (NO DIGITS)
		if (ft_isalpha(cmd->args[i][0]) == 0 && cmd->args[i][0] != '_')
			return (1);
		//goo thru whole arg and check if anything not a-z or 0-9 or _
		y = 1;
		while (cmd->args[i][y])
		{
			if (ft_isalnum(cmd->args[i][y]) == 0 && cmd->args[i][y] != '_' && cmd->args[i][y] != '=' && cmd->args[i][y] != '"')
				return (1);
			y++;
		}
		i++;
	}
	return (0);
}


char	*get_name_only(char *str) //checks for first = only 
{ //check if = in str
	//if yes substr everything before =
	//if no copy
	int 	index;
	int		len;

	len = ft_strlen(str);
	index = 0;
	while(str[index])
	{
		if (str[index] == '=')
			break;
		index++;
	}
	if (index < len) // = in str
		return (ft_substr(str, 0, index));
	if (index == len)//if index same as len then no = in it
		return (ft_substr(str, 0, len));
	return (NULL);
}

t_env	*check_existing_env(char *arg_name,  t_env *envp)
{
	printf("checking existing in envp\n");

	//char	*node_name;
	t_env	*tmp;
	
	tmp = envp;
	while (tmp)
	{
		//go trhu env list, for each contnt
		//compare key to arg name
		if (ft_strncmp((char *)tmp->key, arg_name, ft_strlen(arg_name)) == 0)
		{	
			if (ft_strlen((char *)tmp->key) == ft_strlen(arg_name))
			{
				//if same return list node
				//free(node_name);
				return (tmp);
			}
		}
		tmp = tmp->next;
	}
	//if no match return NULL
	return (NULL);
}

int	create_new_env(char *new_var, t_env *envp) //connect to ft set env in main.c
{
	t_env	*new_node;
	char	*equal;

	new_node = (t_env *)malloc(sizeof(t_env) * 1);
	if (!new_node)
	{
		//free
		return (1);
	}
	//check if = in str
	equal = ft_strchr(new_var, '=');
	if (!equal)//no = in envp str
	{//assign str as key
		new_node->key = new_var;
		new_node->value = NULL;
	}
	else //equal in str
		set_key_value(new_var, new_node, envp);
	new_node->next = NULL;
	add_env_back(&envp, new_node);
	return (0);
}

char	*get_value_only(char *arg)
{
	int		index;
	char	*value;

	index = 0;
	//count to =
	while (arg[index] != '=')
		index++;

	//substr
	value = ft_substr(arg, index + 1, ft_strlen(arg) - index); //MALLOC
	if(!value)
	 	return (NULL);
	printf("new value: %s\n", value);
	//free og arg
	// free(arg);
	// arg = NULL;
	//return new
	return(value);
}


int	update_existing(t_env	*existing_var, char *arg)
{			//check if need to update
			//update existing node

	char	*equal;
	char	*value;
	
	value = NULL;
	//check if need to update (= inside arg)
	equal = ft_strchr(arg, '=');
	if (!equal)//no = in arg str //no need to update
		return (0);
	//if yes
		//get arg value
	value = get_value_only(arg); //MALLOC
		//free current str at value
	free(existing_var->value);
	//update existing var
	existing_var->value = value;
	return (0);
}

int	update_or_create_env(t_command *cmd, t_env *envp)
{		//for each arg check: (all args valid at this point)
		//1.if already exists
			//if yes (get list node)
			//1.5 if need to update (= inside)
				//if yes update list node
				//if no do nothing
			//if no (create new list node)

	int		i;
	char	*arg_name;
	t_env	*existing_var;
	int		exit_stat;

	i = 1; //skip "export" itself
	while (cmd->args[i]) //go thru args
	{
		//get clear name arg
		arg_name = get_name_only(cmd->args[i]); //MALLOC //=key in env
		if (!arg_name)
		{
			//free and 
			return (1);
		}
		//check if that in env list
		existing_var = check_existing_env(arg_name, envp);
		if (!existing_var) //no match in env list
			exit_stat = create_new_env(cmd->args[i], envp); //create new node in list
		else //if match
			exit_stat = update_existing(existing_var, cmd->args[i]);

		free(arg_name);
		i++;
	}
	return(exit_stat);
}

// //export w/o args: lists all exported env vars
// //export multiple at once should be possible: eg export VAR1="value1" VAR2="value2"
// //usually setenv() but not allowed. -> modify env array manually
int	eexport(t_command *cmd, t_env *envp)
{
	int 	num_args;
	int		exit_stat;

	exit_stat = 0;
	num_args = get_num_args(cmd->args);
	printf("nr export args is: %i\n", num_args);
	if (num_args == 1) //only export, man export: When no arguments are given, the results are unspecified.
	{ //just print list of exported vars
		exit_stat = print_env(envp); //OK
	}
	else if (num_args > 1)
	{
		//chck if all args valid (see rules below), else error, stop
		if (check_shellvar_rules(cmd) == 1) //1 if invalid, 0 if valid
		{
			ft_putstr_fd("invalid shell var format\n", 1);//print error
			//free and exit
			//exit(34404);
			return (1); //ggf change nr
		}
		exit_stat = update_or_create_env(cmd, envp);
	}
	else
		printf("sth wrong with cmd list or cmd args\n");
	
	return (exit_stat);

}
//SHELL VAR RULES:  A variable name could contain any alphabet (a-z, A-Z), any digits (0-9), and an underscore ( _ ). However, a variable name must start with an alphabet or underscore. It can never start with a number. 

