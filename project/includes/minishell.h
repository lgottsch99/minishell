/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:55:48 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/14 15:44:00 by lgottsch         ###   ########.fr       */
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

//env list struct
typedef struct s_env {
	char			*key; //eg PATH
	char			*value; // /home/lgottsch/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin
	struct s_env	*next;
} t_env;

//---------- functions ------------------

//amin.c
void	set_key_value(char *env_str,t_env *new_node); // = in str
void	add_env_back(t_env **environ, t_env *new_node);//add new node to end of list


//00_init_start
void	print_start(void);

//builtins
void	print_env(t_env *environ);
void	echo(t_command *cmd_list);
void	pwd(void);
void	exit_shell(void);
void	cd(t_command *cmd_list);
int		get_num_args(char **args);


//execution
void	execute(t_env *envp);

//check_access_exec
char **get_path(t_env *envp);
char 	*get_exec_path(char *cmd, char **path);
int		check_builtin(char *cmd);
int		check_files(t_command *cmd);
int		check_access(t_command	*cmd_list, int nr_cmd, t_env *envp);
void	check_path(t_command	*cmd, t_env *envp);
char 	**convert_env_array(t_env *envp);
int		count_env_size(t_env *envp);

//redirections
void	red_infile(char	*input_file);
void	red_outfile(char *output_file, t_command *cmd);
void	redirect(int fd, int fd_to_replace);

//single_builtin
void	only_builtin(t_command *cmd_list, t_env *envp);
void	run_builtin(t_command *cmd_list, t_env *envp);

//alloc_free_exec
void	free_2d_array(int **fd_pipe, int size);
int		**alloc_fd(int nr_cmd);
int		*alloc_pid(int nr_cmd);

//export.c
void	eexport(t_command *cmd, t_env *envp);
t_env	*check_existing_env(char *arg_name,  t_env *envp);

//unset.c
void	unset(t_command *cmd, t_env *envp);


#endif