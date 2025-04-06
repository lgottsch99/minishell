/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:55:48 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 13:53:43 by lgottsch         ###   ########.fr       */
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

extern volatile sig_atomic_t	g_signal_status;

//---------- Structs ------------------

typedef enum s_token_type
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
}	t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}	t_token;

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

typedef struct s_env_var_context
{
	char	**start;
	char	**end;
	t_token	**head;
	t_token	**current;
	int		last_exit_status;
	char	**envp;
	int		create_flag;
}	t_envVarContext;

typedef struct s_tokenize_context
{
	char	**start;
	char	**end;
	t_token	**head;
	t_token	**current;
}	t_tokenizeContext;

typedef struct s_quote_context
{
	char			**start;
	char			**end;
	t_token			**head;
	t_token			**current;
	int				last_exit_status;
	char			**envp;
}	t_quote_context;

typedef struct s_tokenize_setup
{
	t_token				*head;
	t_token				*current;
	t_token				*token;
	char				*start;
	char				*end;
	t_tokenizeContext	token_ctx;
	t_envVarContext		env_ctx;
}	t_tokenize_setup;

typedef struct tokenize_data
{
	t_token				*head;
	t_token				*current;
	t_token				*token;
	char				*start;
	char				*end;
	t_envVarContext		env_ctx;
	t_tokenizeContext	token_ctx;
	t_quote_context		quote_ctx;
}	t_tdata;

typedef struct s_parse_ctx
{
	t_command			**cmd;
	t_command			**head;
	t_command			**current;
	int					*count;
}	t_parse_ctx;

//---------- FUNCTIONS ------------------------------

//access.c
int			check_files(t_command *cmd, int *exit_stat);
int			loop_check_access(t_command *tmp, t_env *envp,
				t_pipeline *pipeline);

//alloc_free_exec
int			**alloc_fd(int nr_cmd);
int			*alloc_pid(int nr_cmd);
void		free_everything_pipeline_exit(t_env *envp,
				t_pipeline *pipeline, int stat);
void		free_cmd_list(t_command **cmd_list);
void		remove_heredoc(char **heredoc_file);
//free helpers
void		free_2d_array(int **fd_pipe, int size);
void		free_env_list(t_env **env);
void		free_2d_char(char **array);
void		free_pipe_array(int **fd_pipe, int nr_cmd);

//builtin cd
int			decide_case(t_command *cmd_list, char *home, t_env *envp);

//builtin_echo
int			get_num_args(char **args);
int			echo(t_command *cmd_list);

//builtin exit
int			exit_shell(t_command *cmd, t_env *envp,
				t_pipeline *pipeline, int *exit_stat);
//builtin exit helpers
int			check_only_digits(t_command *cmd);
void		free_rest_exit(t_command *cmd, t_env *envp, int stat);
int			check_first_arg(char *arg);

//builtin export
int			eexport(t_command *cmd, t_env *envp);
t_env		*check_existing_env(char *arg_name, t_env *envp);
//buitlin export helpers
char		*get_value_only(char *arg);
char		*get_name_only(char *str);
int			check_shellvar_rules(char *arg);

//builtin unset
int			unset(t_command *cmd, t_env *envp);

//builtins
void		run_builtin(t_command *cmd_list, t_env *envp, t_pipeline *pipeline,
				int *exit_stat);
int			print_env(t_env *environ);
int			pwd(void);
int			cd(t_command *cmd_list, t_env *envp);

//child.c
void		child_process(t_pipeline *pipeline,
				t_env *envp, int i, t_command *tmp);
void		pipeline_builtin(t_pipeline *pipeline, t_command *tmp,
				int i, t_env *envp);
void		pipeline_exec(t_pipeline *pipeline, t_command *tmp, t_env *envp);

//child redirections
void		redirect_heredoc(t_pipeline *pipeline, int i, t_command *tmp,
				t_env *envp);
void		red_pipeline_in(t_pipeline *pipeline, int i, t_command *tmp,
				t_env *envp);
void		red_pipeline_out(t_pipeline *pipeline, int i, t_command *tmp,
				t_env *envp);
void		red_pipeline_pipe_in(t_pipeline *pipeline, int i, t_env *envp);
void		red_pipeline_pipe_out(t_pipeline *pipeline, int i, t_env *envp);

//child helpers
void		close_child_fds(t_pipeline *pipeline, int i);
void		close_not_needed_pipe(int i, t_pipeline *pipeline);
void		child_redirection(t_command *tmp, int i, t_pipeline *pipeline,
				t_env *envp);

//env setup
t_env		*set_env(char *envp[]);
int			set_key_value(char *env_str, t_env *new_node, t_env *environ);
char		*add_shlvl(char *value);
//env helpers2
int			shlvl(t_env	*new_node, t_env *environ);
void		add_env_back(t_env **environ, t_env *new_node);
char		**convert_env_array(t_env *envp, t_pipeline *pipeline);
char		*ret_value_env(char *key, t_env *envp);
int			count_env_size(t_env *envp);

//exec-path
void		check_path(t_command *cmd, t_env *envp);
char		*create_fullstr(t_env *node);
void		search_path(t_command *cmd, t_env *envp);
char		*get_exec_path(char *cmd, char **path);
//exec path helpers
char		*extend_upper_dir(t_command *cmd);
char		*extend_current_dir(t_command *cmd);

//execution
void		execute(t_env *envp, int *exit_stat, t_command *cmd_list);

void		pipeline(t_command *cmd_list, t_env *envp, int *exit_stat);
//execution helpers
int			get_nr_cmd(t_command *cmd_list);
void		init_pipeline(t_pipeline *pipeline, int nr_cmd,
				t_command *cmd_list, int *exit_stat);
void		create_pipes(t_pipeline *pipeline, t_env *envp);
void		close_parent_fds(t_pipeline *pipeline);
void		wait_children(t_pipeline *pipeline, t_env *envp);

//heredoc_helpers
void		heredoc_input(int fd, char *delimetr);
char		*read_heredoc(char *delimetr, int count);

//init_start
void		print_start(void);
int			init_shell(t_env **environ, char *envp[]);

//redirections
int			red_infile(char	*input_file);
int			red_outfile(char *output_file, t_command *cmd);
int			redirect(int fd, int fd_to_replace);

//single builtin
void		only_builtin(t_command *cmd_list, t_env *envp, int *exit_stat);

//parsing.c //tokenizing
t_token		*tokenize(char *input, int last_exit_status, char **envp);
t_command	*parse_tokens(t_token *tokens);

void		print_tokens(t_token *tokens);
void		free_tokens(t_token *tokens);
void		free_commands(t_command *commands);
void		free_command(t_command *cmd);
void		print_commands(t_command *commands);
char		*read_heredoc(char *delimetr, int count);
void		clean_heredoc(t_command *cmd);
char		**env_to_array(t_env *env);
void		handle_redir_less(char **start, char **end,
				t_token **head, t_token **current);
void		handle_heredoc(char **start, char **end,
				t_token **head, t_token **current);
void		handle_redir_more(char **start, char **end,
				t_token **head, t_token **current);
char		*handle_env_var(t_envVarContext *ctx);
void		handle_double_quote(t_quote_context *ctx);
void		handle_single_quote(char **start, char **end,
				t_token **head, t_token **current);
void		handle_pipe(char **start, char **end,
				t_token **head, t_token **current);
void		add_token(t_token **head, t_token **current, t_token *token);
char		*ft_getenv(char *var_name, char **envp);
t_token		*create_token(char *value, t_token_type type);
void		handle_word(t_tokenizeContext *token_ctx, t_envVarContext *env_ctx);
void		handle_space(t_tdata *data);
char		*generate_exit_status_str(int last_exit_status);
void		update_pointers_after_exit(char **start, char **end);
char		*handle_token_creation(t_token **head, t_token **current,
				char *str, int create_flag);
char		*extract_var_name_and_value(char **end, char **envp);
void		handle_env_var_in_word(t_tokenizeContext *token_ctx,
				t_envVarContext *env_ctx);
void		handle_quoted_value_after_equal(char **end);
void		handle_unquoted_value_after_equal(char **end);
char		*process_value_after_equal(char **start, char **end);
int			process_token(t_token **token, t_parse_ctx *ctx);
int			handle_redirection_parsing(t_token **token, t_parse_ctx *ctx,
				int is_output);
int			handle_heredoc_token(t_token **token, t_parse_ctx *ctx);
void		init_double_quote_handling(t_quote_context *ctx,
				int *buffer_index, int *check);
void		process_double_quote_content(t_quote_context *ctx, char *buffer,
				int *buffer_index, int *check);
void		handle_empty_quotes(t_quote_context *ctx);
void		process_env_var(t_quote_context *ctx, char *buffer, int *buf_idx);

//signals
void		setup_signals(void);
void		handle_sigint(int sig);
void		handle_sigint_heredoc(int sig);

void		handle_pipe_parsing(t_command **head, t_command **current,
				t_command **cmd, int *count);
int			handle_redir(t_command **cmd, t_token **tokens,
				t_command **head, int mode);
int			handle_word_parsing(t_command *cmd, char *value,
				t_command **head);
int			handle_heredoc_parsing(t_command **cmd, t_token **tokens,
				int count, t_command **head);

void		add_argument(t_command *command, char *arg);
int			is_builtin(char *arg);
t_command	*create_command(void);

#endif