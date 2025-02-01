/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:12 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/01 18:49:25 by lgottsch         ###   ########.fr       */
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

void	init_test_one(t_command *one)
{
	one->args = (char **)malloc(sizeof(char *) * 2);
	one->command = "cat";
	one->args[0] = "cat";
	one->args[1] = NULL;
	one->input_file = "test.txt";
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = NULL;
	return;
}

void	init_test_two(t_command *one, t_command *two)
{
	one->args = (char **)malloc(sizeof(char *) * 2);
	one->command = "cat";
	one->args[0] = "cat";
	one->args[1] = NULL;
	one->input_file = "test.txt";
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = two;

	two->args = (char **)malloc(sizeof(char *) * 3);
	two->command = "wc";
	two->args[0] = "wc";
	two->args[1] = "-w";
	two->args[2] = NULL;
	two->input_file = NULL;
	two->output_file = NULL;
	two->append_mode = 0;
	two->exec_path = NULL;
	two->is_builtin = 0;
	two->next = NULL;
	return ;
}

int get_nr_cmd(t_command *cmd_list)
{
	int			nr;
	t_command 	*tmp;

	if (cmd_list)
		nr = 1;
	else
		return (0);
	tmp = cmd_list;
	while (tmp->next)
	{
		nr++;
		tmp = tmp->next;
	}
	return (nr);
}

//check executability of cmds, (bash does not check if arguments to a cmd are valid!)
//create executable path and save in cmd table, else stay at NULL
void	check_path(t_command	*cmd, char *envp[])
{
	char	**path; //whole path from envp
	char	*exec_path; //path that can be exec

	//1. get whole path from env
	path = get_path(envp);
	//2. get executable path if cmd not builtin
	exec_path = get_exec_path(cmd->command, path); //returns malloced str if exists, NULL if not
	//3. save exec path in cmd table
	cmd->exec_path = exec_path;
	if(!exec_path)
		printf("nooo executable path found\n");
	else
		printf("executable path found\n");
}


int	check_access(t_command	*cmd_list, int nr_cmd, char *envp[])//ret 1 if access denied, 0 if ok
{
	int			i;
	int			builtin;
	t_command	*tmp; //to trav list

	tmp = cmd_list;
	i = 0;
	while (i < nr_cmd)	//for each cmd in list
	{
		builtin = 0;
		//CHECK IF CMD IS BUILTIN
		builtin = check_builtin(tmp->command);
		//IF NOT BUILTIN:
		if (builtin == 0)
			check_path(tmp, envp);
		else if (builtin == 1)
		{
			printf("cmd is builtin\n");
			tmp->is_builtin = 1;
		}
		if (!tmp->exec_path && builtin == 0) //cmd not found
		{
			printf("minishell: error: cant find command\n");
			return (1);
		}
		//check access to files
		if (check_files(tmp) == 1)
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

void	execute(char *envp[])
{
	int	nr_cmd;
	int	i;
	int	fd_pipe[2]; //fd[0] for read out, fd[1] for write in
	int	prev_pipe_out; //to save fd to read out for next forked p dup
	int	pid;
	int j;
	t_command	*tmp; //to traverse cmd list 
	
	//----for developing only: create my own sample command-lists
	t_command	one;
	//t_command	two;
	t_command	*cmd_list;
	cmd_list = &one;
	
	init_test_one(&one);
	//---------------
	
	//get size of lists 
	nr_cmd = get_nr_cmd(cmd_list);
	printf("\n\nsize cmd list: %i\n\n", nr_cmd);

	//check access of everything (files + cmds), creates paths, decides if builtin
	if (check_access(cmd_list, nr_cmd, envp) != 0)
	{
		//free everything
		exit(18);
	}
	printf("access ok\n");
	
	//SPECIAL CASE nur ein cmd + builtin: dann kein fork!


	// set up pipes (if cmd is builtin they are forked as well, but might have no effect on the main shell p)
	i = 0;
	tmp = cmd_list;
	while (i <= (nr_cmd - 1)) //main loop for pipes 
	{
		//set up pipe if not the last process
		if (nr_cmd > 1 && i < (nr_cmd - 2)) //only if more than one cmd and not in last 
		{
			if (pipe(fd_pipe) == -1) 
			{
				perror("pipe: ");
				//free + exit
				exit(10);
			}
			printf("pipe created\n");
		}
		//fork process
		if ((pid = fork()) < 0)
		{
			perror("fork error: ");
			//free + exit
			exit(11);
		}
		else if (pid == 0) //in child to exec cmd
		{
			int	in; 
			int	out;

			in = 0;
			out = 0;
			printf("in child\n");
			//close not needed pipe ends
			if (nr_cmd > 1 && i < nr_cmd - 1) //if pipes there AND not the last p
				close(fd_pipe[0]);
			//check in/out file
			if (tmp->infile)
			{
				red_in();
				in = 1;
			}
			if (tmp->outfile)
			{
				red_out();
				out = 1;
			}
			//redirect in/out to next pipe (if not already by file, file > pipe redirection)
			if (in == 0 && i > 0) //redirect to 
					//exec and leave process
	


		}
		else // in parent 
		{
			//save read out end of pipe for next process
			prev_pipe_out = fd_pipe[0];
			//close unnecessary pipe ends
			close(fd_pipe[0]);
			close(fd_pipe[1]);
		}
	
	tmp = tmp->next;
	i++;
	}
	
	//wait for all p
	j = 0;
	while (j < (nr_cmd - 1))
	{
		wait(NULL);
		j++;
	}

	printf("waited for all ps and finished\n");

	//random to silence comp warning
	print_env(envp);
}