/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:12 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/27 16:49:54 by lgottsch         ###   ########.fr       */
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

void	pipeline(t_command *cmd_list, int nr_cmd, t_env *envp, int *exit_stat); //works for 2 -> n cmds 


void	init_cd(t_command *one)
{
	one->args = (char **)malloc(sizeof(char *) * 4);
	//one->command = "cd";
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
	//one->command = "env";
	one->args[0] = "env";
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

void	init_test_unset(t_command *one)
{
	one->args = (char **)malloc(sizeof(char *) * 3);
	//one->command = "unset";
	one->args[0] = "unset";
	one->args[1] = "SHLVL";
	one->args[2] = "hiiii";
	one->args[3] = NULL;
	one->input_file = NULL;
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = NULL;
	return;
}

void	init_test_export(t_command *one)
{
	one->args = (char **)malloc(sizeof(char *) * 5);
	//one->command = "export";
	one->args[0] = "export";
	one->args[1] = "lilli=\"cool\"";
	one->args[2] = "lillian=cooler=jik";
	one->args[3] = "hello=world";
	one->args[4] = NULL;
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
	one->args = (char **)malloc(sizeof(char *) * 3);
	//one->command = "exit";
	one->args[0] = "exit";
	one->args[1] = "34";
	one->args[2] = NULL;
	one->input_file = "test.txt";
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = NULL;
	return;
}

void	init_test_four(t_command *one, t_command *two, t_command *three, t_command *four)
{
	one->args = (char **)malloc(sizeof(char *) * 2);
	//one->command = "ls";
	one->args[0] = "ls";
	one->args[1] = NULL;
	one->input_file = NULL;
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = two;

	two->args = (char **)malloc(sizeof(char *) * 3);
	//two->command = "wc";
	two->args[0] = "wc";
	two->args[1] = "-w";
	two->args[2] = NULL;
	two->input_file = NULL;
	two->output_file = NULL;
	two->append_mode = 0;
	two->exec_path = NULL;
	two->is_builtin = 0;
	two->next = three;

	three->args = (char **)malloc(sizeof(char *) * 2);
	//three->command = "cat";
	three->args[0] = "cat";
	three->args[1] = NULL;
	three->input_file = NULL;
	three->output_file = "wc.txt";
	three->append_mode = 1;
	three->exec_path = NULL;
	three->is_builtin = 0;
	three->next = four;


	four->args = (char **)malloc(sizeof(char *) * 3);
	//four->command = "cat";
	four->args[0] = "cat";
	four->args[1] = "wc.txt";
	four->args[2] = NULL;
	four->input_file = NULL;
	four->output_file = NULL;
	four->append_mode = 1;
	four->exec_path = NULL;
	four->is_builtin = 0;
	four->next = NULL;

	return ;
}


void	init_test_three(t_command *one, t_command *two, t_command *three)
{
	one->args = (char **)malloc(sizeof(char *) * 2);
	//one->command = "ls";
	one->args[0] = "ls";
	one->args[1] = NULL;
	one->input_file = NULL;
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = two;

	two->args = (char **)malloc(sizeof(char *) * 2);
	//two->command = "cat";
	two->args[0] = "cat";
	two->args[1] = NULL;
	two->input_file = NULL;
	two->output_file = NULL;
	two->append_mode = 0;
	two->exec_path = NULL;
	two->is_builtin = 0;
	two->next = three;

	three->args = (char **)malloc(sizeof(char *) * 2);
	//three->command = "cat";
	three->args[0] = "cat";
	three->args[1] = NULL;
	three->input_file = NULL;
	three->output_file = "outfile.txt";
	three->append_mode = 0;
	three->exec_path = NULL;
	three->is_builtin = 0;
	three->next = NULL;

	return ;
}


void	init_test_two(t_command *one, t_command *two)
{
	one->args = (char **)malloc(sizeof(char *) * 2);
	//one->command = "cat";
	one->args[0] = "cat";
	one->args[1] = NULL;
	one->input_file = "test.txt";
	one->output_file = NULL;
	one->append_mode = 0;
	one->exec_path = NULL;
	one->is_builtin = 0;
	one->next = two;

	two->args = (char **)malloc(sizeof(char *) * 3);
	//two->command = "cd";
	two->args[0] = "cd";
	two->args[1] = "..";
	two->args[2] = NULL;
	two->input_file = NULL;
	two->output_file = NULL; //"out.txt";
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

void	execute(t_env *envp, int *exit_stat, t_command *cmd_list)
{
	int	nr_cmd;
	//----for developing only: create my own sample command-lists
	// t_command	one;
	// t_command	two;
	//  t_command	three;
	// // t_command	four;

	// t_command	*cmd_list;
	// cmd_list = &one;
	
	// //init_single_builtin(&one);//, &two);
	// init_test_three(&one, &two, &three);//, &four);
	//---------------
	
	//get size of lists 
	nr_cmd = get_nr_cmd(cmd_list);
	printf("\n\nsize cmd list: %i\n\n", nr_cmd);

	//check access of everything (files + cmds), creates paths, decides if builtin
	if (check_access(cmd_list, nr_cmd, envp) != 0)
	{
		//printf("access error\n");
		free_env_list(&envp);
		//free more?
		*exit_stat = 1;
		return;// TO DO 
	}
	printf("access ok\n");
	
	//SPECIAL CASE nur ein cmd + builtin: dann kein fork!
	if (nr_cmd == 1 && cmd_list->is_builtin == 1)
		*exit_stat = only_builtin(cmd_list, envp); //return and set exit stat after onlz builtin TO DO
	// set up pipes (if cmd is builtin they are forked as well, but might have no effect on the main shell p)
	else
		pipeline(cmd_list, nr_cmd, envp, exit_stat);

	return;
}


void	pipeline(t_command *cmd_list, int nr_cmd, t_env *envp, int *exit_stat) //works for 2 -> n cmds 
{
	printf("in pipeline\n");

	int			i;
	int			y; 
	t_command	*tmp; //to traverse cmd list 
	t_pipeline	pipeline;

	pipeline.fd_pipe = alloc_fd(nr_cmd, envp); //MALLOC
	pipeline.pid = alloc_pid(nr_cmd, envp); //MALLOC
	pipeline.env_array = NULL;
	pipeline.cmd_list = cmd_list;

	//1. create ALLLL pipes necessary
	i = 0;
	while (i < nr_cmd - 1)
	{
		if (pipe(pipeline.fd_pipe[i]) == -1)
		{
			perror("pipe: ");
			free_everything_pipeline_exit(envp, &pipeline);
			//free more?
		}
		i++;
	}

	i = 0;
	tmp = pipeline.cmd_list;
	while (i < nr_cmd) //main loop for fork, exec 
	{
		printf("i is: %i\n", i);

		//fork process
		if ((pipeline.pid[i] = fork()) < 0)
		{
			perror("fork error: ");
			free_everything_pipeline_exit(envp, &pipeline);
			//free more?
		}
		if (pipeline.pid[i] == 0) //in child to exec cmd
		{
			//child_process();
			//printf("\nthis is process nr: %i\n\n", i);

			/// i is index of p
			int	in; 
			int	out;

			in = 0;
			out = 0;
			printf("in child\n");

			//close ALLLLLL not needed pipe ends
				y = 0;
				//looop thru fd structure
				if (i == 0) //first cmd
				{
					//close all ex [i][1]
					while (y < nr_cmd - 1)
					{
						if (y != i)
							close(pipeline.fd_pipe[y][1]);
						close(pipeline.fd_pipe[y][0]);
						y++;
					}
				}
				else if (i == nr_cmd - 1) //last cmd
				{
					//close all except [i - 1][0]
					while (y < nr_cmd - 1)
					{
						close(pipeline.fd_pipe[y][1]);
						if (y != i - 1)
							close(pipeline.fd_pipe[y][0]);
						y++;
					}
				}
				else //inbetween
				{
					//close all except [i - 1][0]  AND [i][1]
					while (y < nr_cmd - 1)
					{
						if (y != i)
							close(pipeline.fd_pipe[y][1]);
						if (y != i - 1)
							close(pipeline.fd_pipe[y][0]);
						y++;
					}
				}
				
			//check in/out file
			if (tmp->input_file) //no reading from previous pipe
			{
				if (nr_cmd > 1 && i > 0) //if not in first
					close(pipeline.fd_pipe[i - 1][0]);
				
				red_infile(tmp->input_file);
				in = 1;
				printf("infile redirected\n");
			}
			if (tmp->output_file) //we will not write to pipe
			{
				if (nr_cmd > 1 && i < nr_cmd - 1)
					close(pipeline.fd_pipe[i][1]);

				printf("outfile redirected\n");
				red_outfile(tmp->output_file, tmp);
				out = 1;
			}
			//redirect in/out to next pipe (if not already by file, file > pipe redirection)
			if (in == 0 && i > 0) //redirect in to be read out end of prev pipe
			{
				redirect(pipeline.fd_pipe[i - 1][0], STDIN_FILENO);
				printf("input is prev pipe\n");
			}
			if (out == 0 && i < (nr_cmd - 1))//redirect out to be write in end of current pipe
			{
				printf("output is pipe\n");
				redirect(pipeline.fd_pipe[i][1], STDOUT_FILENO);
			}

			//exec and leave process 
			//if NOT BUILTIN
			if (tmp->is_builtin == 0)
			{
				//printf("going to execve\n");
			//convert t_list envp into char ** for execve
				pipeline.env_array = convert_env_array(envp, &pipeline); //MALLOC
				if (!pipeline.env_array)
				{
					perror("malloc env_array: ");
					free_everything_pipeline_exit(envp, &pipeline);
					//free more?
				}
				printf("converted env to array\n");

				print_commands(tmp);
				
				if(execve(tmp->exec_path, tmp->args, pipeline.env_array) == -1) //execve closing open fds? - yes
				{
					perror("execve: \n");
					free_2d_char(pipeline.env_array);
					free_everything_pipeline_exit(envp, &pipeline);
					///free more?
				}
			}
			//If BUILTIN //TODO test
			else
			{
				printf("its a builtin\n");
				*exit_stat = run_builtin(tmp, envp, &pipeline); //return 0 or error nr?
				
				//close open fds
				if (i == 0) //first cmd
					close(pipeline.fd_pipe[i][1]);
				else if (i == nr_cmd - 1) //last cmd
					close(pipeline.fd_pipe[i - 1][0]);
				else //inbetween
				{
					close(pipeline.fd_pipe[i - 1][0]);
					close(pipeline.fd_pipe[i][1]);
				}
				//free
				free_everything_pipeline_exit(envp, &pipeline);
				//free cmd_list 

				exit(*exit_stat); //use returned stat here? TODO
			}

		}
		else // in parent 
		{
			tmp = tmp->next;
			i++;
		}
	}
	//close all open fildes
	printf("closing fds\n");

	y = 0;
	while (y < nr_cmd - 1)
	{
		close(pipeline.fd_pipe[y][1]);
		close(pipeline.fd_pipe[y][0]);
		y++;
	}


	//wait for all children TO DO whatif terminated by SIGNAL? WTERMSIG
	y = 0;
	while (y < nr_cmd)
	{
		printf("waiting\n");
		//wait(exit_stat);//save exit stat
		if (waitpid(pipeline.pid[y], exit_stat, 0) == -1)
		{
			perror("waitpid: ");
			free_everything_pipeline_exit(envp, &pipeline);
			//return ? or exit
			exit(1);
		}
		//extract real exit_stat + save in env??
		if (WIFEXITED(*exit_stat))
			*exit_stat = WEXITSTATUS(*exit_stat); //check + test if ok?

		y++;
	}	// make sure last process exit is stored????
	
	//free everything malloced for pipeline TO DO
		//free_everything_pipeline_exit(envp, &pipeline);


	printf("waited for all ps and finished\n");
	return;
}