/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/08 15:56:40 by lgottsch         ###   ########.fr       */
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

void	print_env(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
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

//export w/o args: lists all exported env vars
//export multiple at once should be possible: eg export VAR1="value1" VAR2="value2"
//usually setenv() but not allowed. -> modify env array manually
void export()
{

}

/* use unlink()
*/
void unlink()
{
	
}