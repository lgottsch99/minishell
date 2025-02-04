/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/04 17:24:06 by lgottsch         ###   ########.fr       */
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
	//printf("doneeeeeeeeeeeeee\n");
	return;
}

void	pwd(void) //test with very long path TODO
{
	char *path = NULL;

	path = getcwd(path, PATH_MAX);
	// if (!path)
	// 	//TO DO 
	// 	perror("error pwd: ");
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

	printf("exit\n"); //bash prints exit 
	exit(0);
}

void	cd(t_command *cmd_list)//TO DO
{
	//only cd -> goes to home 

	//cd fhksh/gdf/ -> goes 
	if (chdir(path) != 0)
		perror("cd: ");

	
}