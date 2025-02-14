/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/14 13:57:11 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
typedef struct s_command {
    char	*command;       // The command name (e.g., "echo", "grep")
    char	**args;         // Array of arguments (NULL-terminated)
    char	*input_file;    // File for input redirection (NULL if none)
    char	*output_file;   // File for output redirection (NULL if none)
    int		append_mode;     // 1 if output should be appended, 0 otherwise
	//..more if needed:
	char 	*exec_path; // NULL for parsing, execution: saves executable path in here
	int		is_builtin;	//0 for parsing, exec: 0 if not, 1 if yes

    struct s_command *next; // Pointer to the next command in a pipeline
} t_command;

*/


#include "../includes/minishell.h"

// void	print_env(t_list *envp[])//TO DO: CHANGE TO OWN ENV LIST
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		printf("%s\n", envp[i]);
// 		i++;
// 	}
// 	return;
// }

void	print_env(t_env *environ)
{
	t_env	*tmp;

	tmp = environ;
	while (tmp)
	{
		printf("%s", (char *)tmp->key);
		printf("=\"");
		printf("%s\"\n", (char *)tmp->value);
		tmp = tmp->next;
	}
	return;
}


void	pwd(void) //test with very long path TODO
{//if pwd hjfdh sdj  also works just like pwd ( doesnt care about args)
	char *path = NULL;

	path = getcwd(path, PATH_MAX);
	if (!path)
	{
	 	perror("error pwd: ");
		//free everything 
		exit(3246);
	}
	printf("%s\n", path);
	if (path)
		free(path);
	return;
}

int	get_num_args(char **args)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = args;
	while (tmp[i])
		i++;
	printf("num args is: %i\n", i);
	return (i);
}

void	print_args(char **args, int start)
{
	int		i;
	char **tmp;

	i = start;
	tmp = args;
	while (tmp[i])
	{
		ft_putstr_fd(tmp[i], STDOUT_FILENO);
		ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	return;
}

void	echo(t_command *cmd_list)
{
	//printf("in echo\n");
	int num_args;

	num_args = get_num_args(cmd_list->args);
	//check num of args
	if (num_args <= 1) //only echo
		ft_putstr_fd("\n", STDOUT_FILENO);
	else
	{
		//check if -n or not 
		if (ft_strncmp(cmd_list->args[1], "-n", ft_strlen(cmd_list->args[1])) == 0)
		{
			//print all other args starting at indx 2
			if (num_args > 2)
				print_args(cmd_list->args, 2);
			//NO NEW LINE AT END
		}
		else
		{
			//print all other args starting at indx 1
			print_args(cmd_list->args, 1);
			//INCL NL
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
	}
	return;
}



void exit_shell(void)//TODO
{
	//a number after exit can be set which refers to exit status: check $?
	//free all necessary

	// if (fd_pipe)
	// 	free_2d_array(fd_pipe);

	printf("exit\n"); //bash prints exit 
	exit(0);
}

void	cd(t_command *cmd_list)//TO DO
{
	char s[100];
	printf("in cd\n");
	printf("current dir: %s\n", getcwd(s, 100));

	char	*home;
	int		num_args;

	home = getenv("HOME");
	if (!home)
		printf("cant find home\n");
	num_args = get_num_args(cmd_list->args);
	if (num_args > 2) //OK
	{	
		printf("cd: too many args\n");
		//free nd exit //TODO
		exit(19);
	}
	if (!cmd_list->args[1] && home) //only cd -> goes to home //OK
	{
		printf("only cd\n");
		if (chdir(home) == -1)
			perror("cd: ");
	}
	else //cd fhksh/gdf/ -> goes to that dir
	{
		printf("going to cd hghjj\n");

		if (chdir(cmd_list->args[1]) == -1)
		{
			perror("cd: ");
			//free nd exit //TODO
			exit(19);
		}
	}
	printf("new dir: %s\n", getcwd(s, 100));

	return;

}

void	print_list(t_list *envp)
{
	t_list	*tmp;

	tmp = envp;
	while(tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

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
// 			if (ft_isalnum(cmd->arg[i][y]) == 0 && cmd->arg[i][y] != '_')
// 				return (1);
// 			y++;
// 		}
// 		i++;
// 	}
// 	return (0);
// }



// void	update_or_create_env(t_command *cmd, t_list *envp)
// {		//for each arg check: (all args valid at this point)
// 		//1.if already exists
// 			//if yes (get list node)
// 			//1.5 if need to update (= inside)
// 				//if yes update list node
// 				//if no do nothing
// 			//if no (create new list node)

// 	int		i;
// 	char	*arg_name;
// 	t_list	*existing_var;

// 	i = 0;
// 	while (cmd->args[i]) //go thru args
// 	{
// 	//go thru env list
// 		//get clear names
// 		arg_




// 		i++;
// 	}




// }

// //export w/o args: lists all exported env vars
// //export multiple at once should be possible: eg export VAR1="value1" VAR2="value2"
// //usually setenv() but not allowed. -> modify env array manually
// void export(t_command *cmd, t_list *envp)
// {
// 	int 	num_args;
// 	t_list 	*tmp;
// 	int		i;

// 	tmp = envp;
// 	num_args = get_num_args(cmd->args);
// 	printf("nr export args is: %i\n", num_args);
// 	if (num_args == 1) //only export, man export: When no arguments are given, the results are unspecified.
// 	{ //just print list of exported vars
// 		print_list(envp);
// 	}
// 	else
// 	{
// 		//chck if all args valid (see rules below), else error, stop
// 		if (check_shellvar_rules(cmd) == 1) //1 if invalid, 0 if valid
// 		{
// 			ft_putstr_fd("not valid shell var format\n", 1);//print error
// 			//free and exit
// 			exit(34404);
// 		}
// 		update_or_create_env(cmd, env);
// 	}
// }
// //SHELL VAR RULES:  A variable name could contain any alphabet (a-z, A-Z), any digits (0-9), and an underscore ( _ ). However, a variable name must start with an alphabet or underscore. It can never start with a number. 



// /* use unlink()
// */
// void unlink()
// {
	
// }