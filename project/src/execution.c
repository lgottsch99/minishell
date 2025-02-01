//HEADER

/*
typedef struct s_command {
    char	*command;       // The command name (e.g., "echo", "grep")
    char	**args;         // Array of arguments (NULL-terminated)
    char	*input_file;    // File for input redirection (NULL if none)
    char	*output_file;   // File for output redirection (NULL if none)
    int		append_mode;    // 1 if output should be appended, 0 otherwise
	//..more if needed
	char	*exec_path;		// NULL for parsing, execution: saves executable path in here


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
			printf("cmd is builtin\n");
		
		//check access to files
		
		tmp = tmp->next;
		i++;
	}
	
	return (0);
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
	
	init_test_one(&one);
	//---------------
	
	//get size of lists
	nr_cmd = get_nr_cmd(cmd_list);
	printf("\n\nsize cmd list: %i\n\n", nr_cmd);

	//check access of everything (files + cmds)
	if (check_access(cmd_list, nr_cmd, envp) != 0)
	{
		//free everything
		exit(18);
	}

	// check size list, set up pipes (if cmd is builtin they are forked as well, but might have no effect on the main shell p)
		
	// //special case no pipe here
	// if (nr_cmd == 1) //no pipe
	// {
	// 	//check in/outfile red
	// 	//exec
	// 	//break out of loop
	// }

	// i = 0;
	// while (i < (nr_cmd - 1)) //main loop for pipes 
	// {
	// 	//set up pipe if not the last process
	// 	if (pipe(fd_pipe) == -1) 
	// 	{
	// 		perror("pipe: ");
	// 		//free + exit
	// 		exit(10);
	// 	}
	// 	//fork process
	// 	if ((pid = fork()) < 0)
	// 	{
	// 		perror("fork error: ");
	// 		//free + exit
	// 		exit(11);
	// 	}
	// 	else if (pid == 0) //in child to exec cmd
	// 	{
	// 		close(fd_pipe[0]);
	// 		//redirect i/o according to pipeline
	// 		else if (i == 0 && nr_cmd > 1) //first 
	// 		{
	// 			//close not needed pipe ends
	// 			//check in/out file
	// 			//redirect in/out to next pipe
	// 						//exec and leave process
	// 		}
	// 		else if (i == (nr_cmd - 1)) //last 
	// 		{
	// 			//close not needed pipe ends
	// 			//check in/out file
	// 			//redirect in/out to next pipe
	// 						//exec and leave process
	// 		}
	// 		else if (i > 0 && nr_cmd > 1) //inbetween 
	// 		{
	// 			//close not needed pipe ends
	// 			//check in/out file
	// 			//redirect in/out to next pipe
	// 						//exec and leave process

	// 		}
	// 	}
	// 	else // in parent 
	// 	{
	// 		//close unnecessary pipe ends
	// 	}
	

	// i++;
	// }
	
	// //wait for all p
	// j = 0;
	// while (j < (nr_cmd - 1))
	// {
	// 	wait(NULL);
	// 	j++;
	// }

	printf("waited for all ps and finished\n");

	//random to silence comp warning
	print_env(envp);
}