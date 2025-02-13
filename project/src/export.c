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

// #include "../includes/minishell.h"

// //SHELL VAR RULES:  A variable name could contain any alphabet (a-z, A-Z), any digits (0-9), and an underscore ( _ ). However, a variable name must start with an alphabet or underscore. It can never start with a number. 
// int	check_shellvar_rules(t_command *cmd)
// {//chck if all args valid (see rules below), else error, stop
// 	int	i;
// 	int	y;
	
// 	i = 0;
// 	y = 0;
// 	while (cmd->args[i])
// 	{
// 		//check char[0]: if not a-z or '_' (NO DIGITS)
// 		if (ft_isalpha(cmd->arg[i][0]) == 0 && cmd->arg[i][0] != '_')
// 			return (1);
// 		//goo thru whole arg and check if anything not a-z or 0-9 or _
// 		y = 1;
// 		while (cmd->arg[i][y])
// 		{
// 			if (ft_isalnum(cmd->arg[i][y]) == 0 && cmd->arg[i][y] != '_' && cmd->arg[i][y] != '=' && cmd->arg[i][y] != '"')
// 				return (1);
// 			y++;
// 		}
// 		i++;
// 	}
// 	return (0);
// }


// char	*get_name_only(char *str) //checks for first = only 
// { //check if = in str
// 	//if yes substr everything before =
// 	//if no copy
// 	char	*name;
// 	int 	index;
// 	int		len;

// 	len = ft_strlen(str);
// 	index = 0;
// 	while(str[index])
// 	{
// 		if (str[index] == '=')
// 			break;
// 		index++;
// 	}
// 	if (index < len) // = in str
// 		return (ft_substr(str, 0, index));
// 	if (index == len)//if index same as len then no = in it
// 		return (ft_substr(str, 0, len));
// 	return (NULL);
// }

t_env	*check_existing_env(char *arg_name,  t_env *envp)
{
	char	*node_name;
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
				free(node_name);
				return (tmp);
			}
		}
		tmp = tmp->next;
	}
	//if no match return NULL
	return (NULL);
}

void	create_new_env(char *new_var, t_env *envp) //connect to ft set env in main.c
{
	t_env	*new_node;
	char	*equal;

	new_node = (t_env *)malloc(sizeof(t_env) * 1);
	//if (!new_node)
		//free and exit
	//check if = in str
		equal = ft_strchr(new_var, '=');
		if (!equal)//no = in envp str
		{//assign str as key
			new_node->key = new_var;
			new_node->value = NULL;
		}
		else //equal in str
			set_key_value(new_var, new_node);
		new_node->next = NULL;
		add_env_back(&envp, new_node);
}

void	update_existing(t_env	*existing_var, char *arg)
{
	char	*equal;
	
	//check if need to update (= inside arg)
	equal = ft_strchr(arg, '=');
	if (!equal)//no = in arg str //no need to update
		return;
	//if yes
		//get arg value
		//free current str at value
		//update existing var

}

void	update_or_create_env(t_command *cmd, t_env *envp)
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

	i = 0;
	while (cmd->args[i]) //go thru args
	{
		//get clear name arg
		arg_name = get_name_only(cmd->args[i]); //MALLOC //=key in env
		//if (!arg_name)
			//free and exit
		//check if that in env list
		existing_var = check_existing_env(arg_name, envp);
		if (!existing_var) //no match in env list
			create_new_env(cmd->args[i], envp); //create new node in list
		else //if match
			update_existing(existing_var, cmd->args[i]); //TODO
			//check if need to update
			//update existing node

		free(arg_name);
		i++;
	}

}

// //export w/o args: lists all exported env vars
// //export multiple at once should be possible: eg export VAR1="value1" VAR2="value2"
// //usually setenv() but not allowed. -> modify env array manually
void export(t_command *cmd, t_env *envp)
{
	int 	num_args;

	num_args = get_num_args(cmd->args);
	printf("nr export args is: %i\n", num_args);
	if (num_args == 1) //only export, man export: When no arguments are given, the results are unspecified.
	{ //just print list of exported vars
		print_env(envp);
	}
	else
	{
		//chck if all args valid (see rules below), else error, stop
		if (check_shellvar_rules(cmd) == 1) //1 if invalid, 0 if valid
		{
			ft_putstr_fd("not valid shell var format\n", 1);//print error
			//free and exit
			exit(34404);
		}
		update_or_create_env(cmd, env);
	}
}
//SHELL VAR RULES:  A variable name could contain any alphabet (a-z, A-Z), any digits (0-9), and an underscore ( _ ). However, a variable name must start with an alphabet or underscore. It can never start with a number. 

