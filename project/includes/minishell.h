/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:55:48 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/05 17:06:32 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

//---------- external libraries ------------------
#include <stdio.h>  	//printf, readline
#include <unistd.h> 	//isatty, exit
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>		//malloc, free
#include "../lib/full_libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h> 


//---------- Macros ------------------



//---------- Structs ------------------

typedef struct s_command {
    char	*command;       // The command name (e.g., "echo", "grep")
    char	**args;         // Array of arguments (NULL-terminated) and flags
    char	*input_file;    // File for input redirection (NULL if none)
    char	*output_file;   // File for output redirection (NULL if none)
    int		append_mode;    // 1 if output should be appended, 0 otherwise
	//..more if needed:
	char 	*exec_path; // NULL for parsing, execution: saves executable path in here
	int		is_builtin;	//0 for parsing, exec: 0 if not, 1 if yes

    struct s_command *next; // Pointer to the next command in a pipeline
} t_command;


//---------- functions ------------------

//00_init_start
void	print_start(void);

//builtins
void	print_env(char *envp[]);
void	echo(t_command *cmd_list);
void	pwd(void);
void	exit_shell(void);
void	cd(t_command *cmd_list);

//execution
void	execute(char *envp[]);

//check_access_exec
char 	**get_path(char *envp[]);
char 	*get_exec_path(char *cmd, char **path);
int		check_builtin(char *cmd);
int		check_files(t_command *cmd);
int		check_access(t_command	*cmd_list, int nr_cmd, char *envp[]);
void	check_path(t_command	*cmd, char *envp[]);

//redirections
void	red_infile(char	*input_file);
void	red_outfile(char *output_file, t_command *cmd);
void	redirect(int fd, int fd_to_replace);

//single_builtin
void	only_builtin(t_command *cmd_list, char *envp[]);
void	run_builtin(t_command *cmd_list, char *envp[]);



#endif