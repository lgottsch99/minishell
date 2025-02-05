/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:12 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/05 17:29:45 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	TO DO:	pipeline:
					Rethink fildes structure, for n-1 pipes !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					differentiate builtin

				single builtin:
					rest of functions?


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

void	pipeline(t_command *cmd_list, int nr_cmd, char * envp[]);


void	init_cd(t_command *one)
{
	one->args = (char **)malloc(sizeof(char *) * 4);
	one->command = "cd";
	one->args[0] = "cd";
	one->args[1] = "..";
	one->args[2] = "hi";
	one->args[3] = NULL;
	one->input_file = NULL;
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = NULL;
	return;
}


void	init_single_builtin(t_command *one)
{
	one->args = (char **)malloc(sizeof(char *) * 2);
	one->command = "pwd";
	one->args[0] = "pwd";
	one->args[1] = NULL;
	//one->args[3] = NULL;
	one->input_file = NULL;
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = NULL;
	return;
}


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



void	execute(char *envp[])
{
	int	nr_cmd;
	// int	i;
	// int	fd_pipe[2]; //fd[0] for read out, fd[1] for write in
	// int	prev_pipe_out; //to save fd to read out for next forked p dup
	// int	pid;
	// int j;
	// t_command	*tmp; //to traverse cmd list 
	
	//----for developing only: create my own sample command-lists
	t_command	one;
	//t_command	two;
	t_command	*cmd_list;
	cmd_list = &one;
	
	//init_single_builtin(&one);//, &two);
	init_cd(&one);
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
	if (nr_cmd == 1 && cmd_list->is_builtin == 1)
		only_builtin(cmd_list, envp);
	// set up pipes (if cmd is builtin they are forked as well, but might have no effect on the main shell p)
	else
		pipeline(cmd_list, nr_cmd, envp);

	//exit (0);
	return;
}



void	pipeline(t_command *cmd_list, int nr_cmd, char *envp[]) //ONLY 2 Ps FOR NOW
{
	int	i;
	int	fd_pipe[2]; //fd[0] for read out, fd[1] for write in
	int	prev_pipe_out; //to save fd to read out for next forked p dup
	int	pid;
	t_command	*tmp; //to traverse cmd list 


	//TO DO:	Rethink fildes structure, for n-1 pipes !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	i = 0;
	tmp = cmd_list;
	while (i <= (nr_cmd - 1)) //main loop for pipes 
	{
		printf("i is: %i\n", i);
		//set up pipe if not the last process
		if (nr_cmd > 1 && i < (nr_cmd - 1)) //only if more than one cmd and not in last 
		{
			printf("creating pipe\n");

			if (pipe(fd_pipe) == -1) 
			{
				perror("pipe: ");
				//free + exit
				exit(10);
			}
		}
		//fork process
		if ((pid = fork()) < 0)
		{
			perror("fork error: ");
			//free + exit
			exit(11);
		}
		if (pid == 0) //in child to exec cmd
		{
			//child_process();
			//printf("\nthis is process nr: %i\n\n", i);

			int	in; 
			int	out;

			in = 0;
			out = 0;
			printf("in child\n");
			//close not needed pipe ends
			if (nr_cmd > 1 && i < nr_cmd - 1) //if pipes there AND not the last p
				close(fd_pipe[0]);
			if (nr_cmd > 1 && i == nr_cmd - 1) //if last p
				close(fd_pipe[1]);
			
			//check in/out file
			if (tmp->input_file) //no reading from previous pipe
			{
				if (nr_cmd > 1 && i > 0) //??? if not in first
					close(prev_pipe_out);
				
				red_infile(tmp->input_file);
				in = 1;
				printf("infile redirected\n");
			}
			if (tmp->output_file) //we will not write to pipe
			{
				if (nr_cmd > 1 && i < nr_cmd && i > 0)
					close(fd_pipe[1]);

				printf("outfile redirected\n");
				red_outfile(tmp->output_file, tmp);
				out = 1;
			}
			//redirect in/out to next pipe (if not already by file, file > pipe redirection)
			if (in == 0 && i > 0) //redirect in to be read out end of prev pipe
			{
				redirect(prev_pipe_out, STDIN_FILENO);
				printf("input is prev pipe\n");
			}
			if (out == 0 && i < (nr_cmd - 1))//redirect out to be write in end of current pipe
			{
				printf("output is pipe\n");
				redirect(fd_pipe[1], STDOUT_FILENO);
			}

			//exec and leave process TO DO
			//if NOT BUILTIN
			if (tmp->is_builtin == 0)
			{
				printf("going to execve\n");

				if(execve(tmp->exec_path, tmp->args, envp) == -1) //?? execve closing open fds?
					exit(100);
			}
			//If BUILTIN TODO
			else
				printf("its a builtin\n");

			exit(400);

			
		}
		else // in parent 
		{
			if (nr_cmd > 1 && i < (nr_cmd - 1))
			{
				if (i > 0)
					close(prev_pipe_out);
				//save read out end of pipe for next process
				if (i < nr_cmd - 1)
					prev_pipe_out = fd_pipe[0];
			}

			tmp = tmp->next;
			i++;
		}
	}
	//close all open fildes
		//TO DO
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	if (nr_cmd > 1)
		close(prev_pipe_out);

	//wait for all children TO DO
	// while (nr_cmd > 0)
	// {
	wait(NULL);
	wait(NULL);

	// 	nr_cmd--;
	// }
	
	
	printf("waited for all ps and finished\n");
	return;
}