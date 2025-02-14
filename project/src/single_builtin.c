/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:04:16 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/14 15:46:09 by lgottsch         ###   ########.fr       */
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

void	run_builtin(t_command *cmd_list, t_env *envp)
{
	printf("choosing builtin ft\n");
	
	if (ft_strncmp(cmd_list->command, "env", ft_strlen(cmd_list->command)) == 0)
		print_env(envp);
	else if (ft_strncmp(cmd_list->command, "echo", ft_strlen(cmd_list->command)) == 0)
		echo(cmd_list);
	else if (ft_strncmp(cmd_list->command, "pwd", ft_strlen(cmd_list->command)) == 0)
		pwd();
	// else if (ft_strncmp(cmd_list->command, "exit", ft_strlen(cmd_list->command)) == 0)
	// 	exit_shell(cmd_list);
	else if (ft_strncmp(cmd_list->command, "cd", ft_strlen(cmd_list->command)) == 0)
		cd(cmd_list);
	else if (ft_strncmp(cmd_list->command, "export", ft_strlen(cmd_list->command)) == 0)
		eexport(cmd_list, envp);
	else if (ft_strncmp(cmd_list->command, "unset", ft_strlen(cmd_list->command)) == 0)
		unset(cmd_list, envp);

	envp = NULL;
	return;

}

void	only_builtin(t_command *cmd_list, t_env *envp) //no need to fork + pipe
{
	printf("running single builtin\n");
	int og_in;
	int og_out;
	int red_in;		//if 0 no red happened, if 1 yes 
	int red_out;	//same

	og_in = dup(STDIN_FILENO); //ADD PROTECT 
	og_out = dup(STDOUT_FILENO); //SAME
	red_in = 0;
	red_out = 0;
	
	// redirect if needed
	if (cmd_list->input_file)
	{
		red_infile(cmd_list->input_file);
		red_in = 1;
	}
	if (cmd_list->output_file)
	{
		red_outfile(cmd_list->output_file, cmd_list);
		red_out = 1;
	}
	//go to function and run
	run_builtin(cmd_list, envp);

	// restore og fildes
	if (red_in == 1)
		redirect(og_in, STDIN_FILENO);
	// else
	// 	close(og_in);
	if (red_out == 1)
		redirect(og_out, STDOUT_FILENO);
	// else
	// 	close(og_out);
	return;
}
