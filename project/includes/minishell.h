/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:55:48 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/28 14:46:58 by lgottsch         ###   ########.fr       */
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

typedef enum Token_type {
    TOKEN_WORD,              
    TOKEN_PIPE,              
    TOKEN_REDIRECT_IN,       
    TOKEN_REDIRECT_OUT,      
    TOKEN_REDIRECT_APPEND,   
    TOKEN_REDIRECT_HEREDOC,  
    TOKEN_SINGLE_QUOTE,      
    TOKEN_DOUBLE_QUOTE,      
    TOKEN_ENV_VAR,           
    TOKEN_END                
} Token_type;

typedef struct Token {
    char *value;
    Token_type type;
    struct Token *next;
} Token;

typedef struct s_command {
   // char	*command;       // The command name (e.g., "echo", "grep")
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

//pipeline everything malloced
typedef struct s_pipeline {
	int			**fd_pipe;
	int 		*pid;
	char		**env_array;
	t_command	*cmd_list;
} t_pipeline;

//---------- functions ------------------

//main.c


//00_init_start
void	print_start(void);
char	*add_shlvl(char *value);
int		set_key_value(char *env_str,t_env *new_node, t_env *environ);
void	add_env_back(t_env **environ, t_env *new_node);//add new node to end of list
t_env	*set_env(char *envp[]); //create linked list w all env vars, increasing shlvl by 1


//builtins
int		print_env(t_env *environ);
int		echo(t_command *cmd_list);
int		pwd(void);
int		exit_shell(t_command *cmd, t_env *envp, t_pipeline *pipeline);
int		cd(t_command *cmd_list, t_env *envp);
int		get_num_args(char **args);


//execution
void	execute(t_env *envp, int *exit_stat, t_command *cmd_list);

//check_access_exec
char 	**get_path(t_env *envp);
char 	*get_exec_path(char *cmd, char **path);
int		check_builtin(char *cmd);
int		check_files(t_command *cmd);
int		check_access(t_command	*cmd_list, int nr_cmd, t_env *envp);
void	check_path(t_command	*cmd, t_env *envp);
char	**convert_env_array(t_env *envp, t_pipeline *pipeline); //The envp array must be terminated by a NULL pointer.
int		count_env_size(t_env *envp);
char	*ret_value_env(char *key, t_env *envp);


//redirections
void	red_infile(char	*input_file);
void	red_outfile(char *output_file, t_command *cmd);
void	redirect(int fd, int fd_to_replace);

//single_builtin
int		only_builtin(t_command *cmd_list, t_env *envp);
int		run_builtin(t_command *cmd_list, t_env *envp, t_pipeline *pipeline);

//alloc_free_exec
void	free_2d_array(int **fd_pipe, int size);
int		**alloc_fd(int nr_cmd, t_env *envp);
int		*alloc_pid(int nr_cmd, t_env *envp);
void	free_env_list(t_env **env);
void	free_2d_char(char **array);
void	free_everything_pipeline_exit(t_env *envp, t_pipeline *pipeline);
void	free_cmd_list(t_command **cmd_list);

//export.c
int		eexport(t_command *cmd, t_env *envp);
t_env	*check_existing_env(char *arg_name,  t_env *envp);

//unset.c
int	unset(t_command *cmd, t_env *envp);

//parsing.c //tokenizing
Token		*tokenize(char *input, int last_exit_status, char **envp);
t_command 	*parse_tokens(Token *tokens);
void		print_tokens(Token *tokens);
void		free_tokens(Token *tokens);
void		free_commands(t_command *commands);
void		print_commands(t_command *commands);




#endif