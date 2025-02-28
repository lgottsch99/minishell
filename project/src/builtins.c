/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/28 14:03:45 by lgottsch         ###   ########.fr       */
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

int	print_env(t_env *environ)
{
	t_env	*tmp;

	tmp = environ;
	while (tmp)
	{
		if (printf("%s", (char *)tmp->key) < 0)
			return (1);
		if (printf("=") < 0)
			return (1);
		if (printf("%s\n", (char *)tmp->value) < 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}


int	pwd(void) //test with very long path TODO
{//if pwd hjfdh sdj  also works just like pwd ( doesnt care about args)
	char *path = NULL;

	path = getcwd(path, PATH_MAX);
	if (!path)
	{
	 	perror("error pwd: ");
		//free everything 
		return (1);
	}
	printf("%s\n", path);
	if (path)
		free(path);
	return (0);
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

int	print_args(char **args, int start)
{
	int		i;
	char **tmp;

	i = start;
	tmp = args;
	while (tmp[i])
	{
		// ft_putstr_fd(tmp[i], STDOUT_FILENO); //Change to printf TO DO
		if (printf("%s", tmp[i]) < 0)
			return (1);
		// ft_putstr_fd(" ", STDOUT_FILENO);
		if (printf(" ") < 0)
			return (1);
		i++;
	}
	return (0);
}

int	echo(t_command *cmd_list)
{
	//printf("in echo\n");
	int num_args;

	num_args = get_num_args(cmd_list->args);
	//check num of args
	if (num_args <= 1) //only echo
	{
		if (printf("\n") < 0)
			return (1);
	}
	else
	{
		//check if -n or not 
		if (ft_strncmp(cmd_list->args[1], "-n", ft_strlen(cmd_list->args[1])) == 0)
		{
			//print all other args starting at indx 2
			if (num_args > 2)
				if (print_args(cmd_list->args, 2) == 1)
					return (1);
			//NO NEW LINE AT END
		}
		else
		{
			//print all other args starting at indx 1
			if (print_args(cmd_list->args, 1) == 1)
				return (1);
			//INCL NL
			// ft_putstr_fd("\n", STDOUT_FILENO);
			if (printf("\n") < 0)
				return (1);
		}
	}
	return (0);
}



int exit_shell(t_command *cmd, t_env *envp, t_pipeline *pipeline)//TODO
{	//a number after exit can be set which refers to exit status
	int	num_args;
	int	i;
	int no_digit;

	printf("exit\n"); //bash prints exit 
	num_args = get_num_args(cmd->args);
	i = 0;
	no_digit = 0;
	
	if (num_args == 1)//only exit but no args
	{ 
		//free all necessary
		if (pipeline != NULL)
			free_everything_pipeline_exit(envp, pipeline);
		else //only single cmd
		{ printf("freeing no pipeline\n");
			// if (envp)
			// 	free_env_list(&envp);
			//printf("freed env\n");
			if (cmd)
				free_cmd_list(&cmd);
			//anything else to free?
			//printf("freed all\n");

		}
		exit(0);
	}
	else if (num_args == 2) //if only one: exit with nr 
	{
		//check if arg nr is only digits 
		while (cmd->args[1][i])
		{
			if (ft_isdigit(cmd->args[1][i]) == 0)
				no_digit = 1;
			i++;
		}
		if (no_digit == 1)//non numeric 
		{
			printf("exit: numeric arg required\n");
			return (1);
		}
		//free all necessary
		if (pipeline)
			free_everything_pipeline_exit(envp, pipeline);
		exit(ft_atoi(cmd->args[1])); //check if ok
	}
	else if (num_args > 2)//if more than one number: error mdg, set exit stat
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	return (1);
}

int	cd(t_command *cmd_list)//TO DO
{
	char s[100];
	printf("in cd\n");
	printf("current dir: %s\n", getcwd(s, 100));

	char	*home;
	int		num_args;

	home = getenv("HOME"); //change to own env //TODO
	if (!home)
	{
		printf("cant find home\n");
		return (1);
	}

	num_args = get_num_args(cmd_list->args);
	if (num_args > 2) //OK
	{	
		printf("cd: too many args\n");
		//free nd 
		return(1);
	}
	if (!cmd_list->args[1] && home) //only cd -> goes to home //OK
	{
		printf("only cd\n");
		if (chdir(home) == -1)
		{
			perror("cd: ");
			return (1);
		}
	}
	else //cd fhksh/gdf/ -> goes to that dir
	{
		printf("going to cd hghjj\n");

		if (chdir(cmd_list->args[1]) == -1)
		{
			perror("cd: ");
			//free nd 
			return (1);
		}
	}
	printf("new dir: %s\n", getcwd(s, 100));

	return (0);

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

