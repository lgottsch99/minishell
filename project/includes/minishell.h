/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:55:48 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/21 20:21:32 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//---------- external libraries ------------------
# include <stdio.h>  	//printf, readline
# include <unistd.h> 	//isatty, exit
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>		//malloc, free
# include "../lib/full_libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h> 
# include <signal.h>
# include <errno.h>
# include <sys/ioctl.h>



//---------- GLOBAL FOR SIGNAL ------------------

extern volatile sig_atomic_t g_signal_status;


//---------- Structs ------------------

typedef enum Token_type
{
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
}	Token_type;

typedef struct Token
{
	char			*value;
	Token_type		type;
	struct Token	*next;
}	Token;

typedef struct s_command
{
	char				**args;// Array of arguments (NULL-terminated) and flags
	char				*input_file;// File for input red (NULL if none)
	char				*output_file;// File for output red (NULL if none)
	int					append_mode;// 1 if output should be appended, else 0
	char				*heredoc_file;//name of heredoc file
	char				*heredoc_delimetr;
	char				*exec_path;// NULL for parsing, exec.: saves exec. path
	int					is_builtin;//0 for parsing, exec: 0 if not, 1 if yes
	struct s_command	*next;// Pointer to the next command in a pipeline
}	t_command;

//env list struct
typedef struct s_env
{
	char			*key;//eg PATH
	char			*value;// /home/lgottsch/bin:/usr/local/sbin:
	struct s_env	*next;
}	t_env;

//pipeline everything malloced
typedef struct s_pipeline
{
	int			**fd_pipe;
	int			*pid;
	char		**env_array;
	t_command	*cmd_list;
	int			nr_cmd;
	int			*exit_stat;
}	t_pipeline;

typedef struct s_single_red
{
	int	og_in;
	int	og_out;
	int	red_in;
	int	red_out;
}	t_single_red;

typedef struct s_env_var_context {
    char **start;
    char **end;
    Token **head;
    Token **current;
    int last_exit_status;
    char **envp;
    int create_flag;
} EnvVarContext;

typedef struct s_tokenize_context {
    char **start;
    char **end;
    Token **head;
    Token **current;
} TokenizeContext;

//---------- FUNCTIONS ------------------------------

//main.c

//access.c
int		check_files(t_command *cmd, int *exit_stat);
int		check_access(t_command	*cmd_list, int nr_cmd,
			t_env *envp, int *exit_stat);

//alloc_free_exec
int		**alloc_fd(int nr_cmd);
int		*alloc_pid(int nr_cmd);
void	free_everything_pipeline_exit(t_env *envp,
			t_pipeline *pipeline, int stat);
void	free_cmd_list(t_command **cmd_list);
void	remove_heredoc(char *heredoc_file);
//free helpers
void	free_2d_array(int **fd_pipe, int size);
void	free_env_list(t_env **env);
void	free_2d_char(char **array);
void	free_pipe_array(int **fd_pipe, int nr_cmd);

//builtin cd
int		decide_case(t_command *cmd_list, char *home, t_env *envp);

//builtin_echo
int		get_num_args(char **args);
int		echo(t_command *cmd_list);

//builtin exit
int		exit_shell(t_command *cmd, t_env *envp,
			t_pipeline *pipeline, int *exit_stat);
	//builtin exit helpers
int		check_only_digits(t_command *cmd);
void	free_rest_exit(t_command *cmd, t_env *envp, int stat);
int		check_first_arg(char *arg);

//builtin export
int		eexport(t_command *cmd, t_env *envp);
t_env	*check_existing_env(char *arg_name, t_env *envp);
	//buitlin export helpers
char	*get_value_only(char *arg);
char	*get_name_only(char *str);
// int		check_shellvar_rules(t_command *cmd);
int		check_shellvar_rules(char *arg);

//builtin unset
int		unset(t_command *cmd, t_env *envp);

//builtins
void	run_builtin(t_command *cmd_list, t_env *envp, t_pipeline *pipeline,
			int *exit_stat);
int		print_env(t_env *environ);
int		pwd(void);
int		cd(t_command *cmd_list, t_env *envp);

//child.c
void	child_process(t_pipeline *pipeline, t_env *envp, int i, t_command *tmp);
void	pipeline_builtin(t_pipeline *pipeline, t_command *tmp,
			int i, t_env *envp);
void	pipeline_exec(t_pipeline *pipeline, t_command *tmp, t_env *envp);

//child redirections
void	redirect_heredoc(t_pipeline *pipeline, int i, t_command *tmp,
			t_env *envp);
void	red_pipeline_in(t_pipeline *pipeline, int i, t_command *tmp,
			t_env *envp);
void	red_pipeline_out(t_pipeline *pipeline, int i, t_command *tmp,
			t_env *envp);
void	red_pipeline_pipe_in(t_pipeline *pipeline, int i, t_env *envp);
void	red_pipeline_pipe_out(t_pipeline *pipeline, int i, t_env *envp);

//child helpers
void	close_child_fds(t_pipeline *pipeline, int i);
void	close_not_needed_pipe(int i, t_pipeline *pipeline);
void	child_redirection(t_command *tmp, int i, t_pipeline *pipeline,
			t_env *envp);

//env setup
t_env	*set_env(char *envp[]);
int		set_key_value(char *env_str, t_env *new_node, t_env *environ);
char	*add_shlvl(char *value);
//env helpers2
int		shlvl(t_env	*new_node, t_env *environ);
void	add_env_back(t_env **environ, t_env *new_node);
char	**convert_env_array(t_env *envp, t_pipeline *pipeline);
char	*ret_value_env(char *key, t_env *envp);
int		count_env_size(t_env *envp);

//exec-path
void	check_path(t_command *cmd, t_env *envp);
char	*create_fullstr(t_env *node);
void	search_path(t_command *cmd, t_env *envp);
char	*get_exec_path(char *cmd, char **path);
//exec path helpers
char	*extend_upper_dir(t_command *cmd);
char	*extend_current_dir(t_command *cmd);

//execution
void	execute(t_env *envp, int *exit_stat, t_command *cmd_list);

void	pipeline(t_command *cmd_list, t_env *envp, int *exit_stat);
//execution helpers
int		get_nr_cmd(t_command *cmd_list);
void	init_pipeline(t_pipeline *pipeline, int nr_cmd,
			t_command *cmd_list, int *exit_stat);
void	create_pipes(t_pipeline *pipeline, t_env *envp);
void	close_parent_fds(t_pipeline *pipeline);
void	wait_children(t_pipeline *pipeline, t_env *envp);

//heredoc_helpers
// void	heredoc_input(int fd, char *delimetr);
int	heredoc_input(int fd, char *delimetr);

char	*read_heredoc(char *delimetr, int count);

//init_start
void	print_start(void);
int		init_shell(t_env **environ, char *envp[]);

//main

//redirections
int		red_infile(char	*input_file);
int		red_outfile(char *output_file, t_command *cmd);
int		redirect(int fd, int fd_to_replace);

//single builtin
void	only_builtin(t_command *cmd_list, t_env *envp, int *exit_stat);

//parsing.c //tokenizing
Token		*tokenize(char *input, int last_exit_status, char **envp);
t_command	*parse_tokens(Token *tokens);

void		print_tokens(Token *tokens);
void		free_tokens(Token *tokens);
void		free_commands(t_command *commands);
void		print_commands(t_command *commands);
char		*read_heredoc(char *delimetr, int count);
void		clean_heredoc(t_command *cmd);
char		**env_to_array(t_env *env);
void		handle_redir_less(char **start, char **end, Token **head, Token **current);
void		handle_heredoc(char **start, char **end, Token **head, Token **current);
void		handle_redir_more(char **start, char **end, Token **head, Token **current);
char		*handle_env_var(EnvVarContext *ctx);
void		handle_double_quote(char **start, char **end, Token **head, Token **current, int last_exit_status, char **envp);
void		handle_single_quote(char **start, char **end, Token **head, Token **current);
void		handle_pipe(char **start, char **end, Token **head, Token **current);
void		add_token(Token **head, Token **current, Token *token);
char		*ft_getenv(char *var_name, char **envp);
Token		*create_token(char *value, Token_type type);
void		handle_word(TokenizeContext *token_ctx, 
				EnvVarContext *env_ctx);
char	*generate_exit_status_str(int last_exit_status);
void	update_pointers_after_exit(char **start, char **end);
char	*handle_token_creation(Token **head, Token **current,
				char *str, int create_flag);
char	*extract_var_name_and_value(char **end, char **envp);
void handle_env_var_in_word(TokenizeContext *token_ctx,
				EnvVarContext *env_ctx);
void handle_quoted_value_after_equal(char **end);
void handle_unquoted_value_after_equal(char **end);
char *process_value_after_equal(char **start, char **end);

//signals
void	setup_signals();
void	handle_sigint(int sig);
void handle_sigint_heredoc(int sig);

#endif