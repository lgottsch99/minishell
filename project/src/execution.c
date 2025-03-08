/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:09:12 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/08 18:37:51 by lgottsch         ###   ########.fr       */
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

	//get size of lists 
	nr_cmd = get_nr_cmd(cmd_list);
	printf("\n\nsize cmd list: %i\n\n", nr_cmd);

	print_commands(cmd_list);
	printf("\n\n");

	// printf("envp: %p\n", envp);
	// printf("cmd : %p\n", cmd_list);
	// printf("nr_cmd : %i\n", nr_cmd);


	//check access of everything (files + cmds), creates paths, decides if builtin
	if (check_access(cmd_list, nr_cmd, envp) != 0)
	{
		printf("access error\n");
		//free_cmd_list(&cmd_list); 		//TODO? 
		//free more?
		*exit_stat = 1;
		return;// TO DO 
	}
	printf("access ok\n");
	
	//SPECIAL CASE nur ein cmd + builtin: dann kein fork!
	if (nr_cmd == 1 && cmd_list->is_builtin == 1)
		*exit_stat = only_builtin(cmd_list, envp);
	// set up pipes (if cmd is builtin they are forked as well, but might have no effect on the main shell p)
	else
		pipeline(cmd_list, nr_cmd, envp, exit_stat);

	return;
}

void	init_pipeline(t_pipeline *pipeline, int nr_cmd, t_command *cmd_list)
{
	if (nr_cmd > 1)
		pipeline->fd_pipe = alloc_fd(nr_cmd); //MALLOC
	else
		pipeline->fd_pipe = NULL;
	//if (nr_cmd > 1)
	pipeline->pid = alloc_pid(nr_cmd); //MALLOC
	//else
	//	pipeline->pid = NULL;
	pipeline->env_array = NULL;
	pipeline->cmd_list = cmd_list;
	pipeline->nr_cmd = nr_cmd;
	// if (!pipeline->fd_pipe || !pipeline->pid) //check if ok with only one cmd TODO
	// {
	// 	printf("Error allocating memory. Exiting minishell\n");
	// 	free_everything_pipeline_exit(envp, pipeline, 1, nr_cmd);
	// } 
}

void	pipeline(t_command *cmd_list, int nr_cmd, t_env *envp, int *exit_stat) //works for 2 -> n cmds 
{
	printf("in pipeline\n");

	int			i;
	int			y; 
	t_command	*tmp; //to traverse cmd list 
	t_pipeline	pipeline;

	init_pipeline(&pipeline, nr_cmd, cmd_list); //MALLOC

	//1. create ALLLL pipes necessary
	i = 0;
	while (i < pipeline.nr_cmd - 1)
	{
		printf("creating piep\n");
		if (pipe(pipeline.fd_pipe[i]) == -1)
		{
			perror("pipe: ");
			free_everything_pipeline_exit(envp, &pipeline, 1);
			//free more?
		}
		i++;
	}

	i = 0;
	tmp = pipeline.cmd_list;
	while (i < pipeline.nr_cmd) //main loop for fork, exec 
	{
		printf("i is: %i\n", i);

		//fork process
		if ((pipeline.pid[i] = fork()) < 0)
		{
			perror("fork error: ");
			free_everything_pipeline_exit(envp, &pipeline, 1);
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
					while (y < pipeline.nr_cmd - 1)
					{
						if (y != i)
							close(pipeline.fd_pipe[y][1]);
						close(pipeline.fd_pipe[y][0]);
						y++;
					}
				}
				else if (i == pipeline.nr_cmd - 1) //last cmd
				{
					//close all except [i - 1][0]
					while (y < pipeline.nr_cmd - 1)
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
					while (y < pipeline.nr_cmd - 1)
					{
						if (y != i)
							close(pipeline.fd_pipe[y][1]);
						if (y != i - 1)
							close(pipeline.fd_pipe[y][0]);
						y++;
					}
				}
			//check if heredoc, if yes normal input red. gets ignored
			if (tmp->heredoc_file)
			{
				if (pipeline.nr_cmd > 1 && i > 0) //if not in first
					close(pipeline.fd_pipe[i - 1][0]);
				
				printf("cmd heredoc file: %s\n", tmp->heredoc_file);
				if (red_infile(tmp->heredoc_file) == 1)
				{
					perror("redirection error: ");
					free_everything_pipeline_exit(envp, &pipeline, 1);
				}
				
				unlink(tmp->heredoc_file);

				in = 1;
				printf("heredoc redirected\n");

			}
			//check in/out file
			else if (tmp->input_file && in == 0) //no reading from previous pipe
			{
				if (pipeline.nr_cmd > 1 && i > 0) //if not in first
					close(pipeline.fd_pipe[i - 1][0]);
				
				if (red_infile(tmp->input_file) == 1)
				{
					perror("redirection error: ");
					free_everything_pipeline_exit(envp, &pipeline, 1);
				}

				in = 1;
				printf("infile redirected\n");
			}
			if (tmp->output_file) //we will not write to pipe
			{
				if (pipeline.nr_cmd > 1 && i < pipeline.nr_cmd - 1)
					close(pipeline.fd_pipe[i][1]);

				printf("outfile redirected\n");
				if (red_outfile(tmp->output_file, tmp) == 1)
				{
					perror("redirection error: ");
					free_everything_pipeline_exit(envp, &pipeline, 1);
				}

				out = 1;
			}
			//redirect in/out to next pipe (if not already by file, file > pipe redirection)
			if (in == 0 && i > 0) //redirect in to be read out end of prev pipe
			{
				if (redirect(pipeline.fd_pipe[i - 1][0], STDIN_FILENO) == 1)
				{
					perror("redirection error: ");
					free_everything_pipeline_exit(envp, &pipeline, 1);
				}
 
				printf("input is prev pipe\n");
			}
			if (out == 0 && i < (pipeline.nr_cmd - 1))//redirect out to be write in end of current pipe
			{
				printf("output is pipe\n");
				if (redirect(pipeline.fd_pipe[i][1], STDOUT_FILENO) == 1)
				{
					perror("redirection error: ");
					free_everything_pipeline_exit(envp, &pipeline, 1);
				}
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
					free_everything_pipeline_exit(envp, &pipeline, 1);
					//free more?
				}
				printf("converted env to array\n");

				print_commands(tmp);
				
				if(execve(tmp->exec_path, tmp->args, pipeline.env_array) == -1) //execve closing open fds? - yes
				{
					perror("execve: \n");
					free_2d_char(pipeline.env_array);
					free_everything_pipeline_exit(envp, &pipeline, 1); //1 as exit tstat ok??
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
				else if (i == pipeline.nr_cmd - 1) //last cmd
					close(pipeline.fd_pipe[i - 1][0]);
				else //inbetween
				{
					close(pipeline.fd_pipe[i - 1][0]);
					close(pipeline.fd_pipe[i][1]);
				}
				//free
				free_everything_pipeline_exit(envp, &pipeline, *exit_stat);
				//free cmd_list 

				//exit(*exit_stat); //use returned stat here? TODO
			}

		}
		else // in parent 
		{
			tmp = tmp->next;
			i++;
		}
	}
	//close all open fildes
	printf("closing pipe fds\n");

	y = 0;
	while (y < pipeline.nr_cmd - 1)
	{
		close(pipeline.fd_pipe[y][1]);
		close(pipeline.fd_pipe[y][0]);
		y++;
	}

	//wait for all children TO DO whatif terminated by SIGNAL? WTERMSIG
	y = 0;
	while (y < pipeline.nr_cmd)
	{
		printf("waiting\n");
		//wait(exit_stat);//save exit stat
		if (waitpid(pipeline.pid[y], exit_stat, 0) == -1)
		{
			perror("waitpid: ");
			free_everything_pipeline_exit(envp, &pipeline, 1);
			//return ? or exit
		}
		//extract real exit_stat + save in env??
		if (WIFEXITED(*exit_stat))
			*exit_stat = WEXITSTATUS(*exit_stat); //check + test if ok?

		y++;
	}	// make sure last process exit is stored????

	
	//free everything malloced for pipeline TO DO
	if (pipeline.fd_pipe)
		free_pipe_array(pipeline.fd_pipe, pipeline.nr_cmd);
	if (pipeline.pid)
		free(pipeline.pid);

	printf("waited for all ps and finished\n");
	return;
}