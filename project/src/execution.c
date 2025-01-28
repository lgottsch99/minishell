//HEADER

/*
typedef struct s_command {
    char	*command;       // The command name (e.g., "echo", "grep")
    char	**args;         // Array of arguments (NULL-terminated)
    char	*input_file;    // File for input redirection (NULL if none)
    char	*output_file;   // File for output redirection (NULL if none)
    int		append_mode;    // 1 if output should be appended, 0 otherwise
	//..more if needed



    struct s_command *next; // Pointer to the next command in a pipeline
} t_command;
*/


#include "../includes/minishell.h"

void	init_test(t_command *one, t_command *two)
{
	one->args = (char **)malloc(sizeof(char *) * 2);
	one->command = "cat";
	one->args[0] = "cat";
	one->args[1] = NULL;
	one->input_file = "test.txt";
	one->output_file = NULL;
	one->append_mode = 0;
	one->next = two;

	two->args = (char **)malloc(sizeof(char *) * 3);
	two->command = "wc";
	two->args[0] = "wc";
	two->args[1] = "-w";
	two->args[2] = NULL;
	two->input_file = NULL;
	two->output_file = NULL;
	two->append_mode = 0;
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


// int	check_access(t_command	*cmd_list)//ret 1 if access denied, 0 if ok
// {
// 	//check executability of cmds, 
// 	//access to files
// 	return (0);
// }

void	execute(char *envp[])
{
	int	nr_cmd;
	//----for developing: create my own sample command-lists
	t_command	one;
	t_command	two;
	t_command	*cmd_list;
	cmd_list = &one;
	
	init_test(&one, &two);
	//---------------

	//0. check access of everything (files + cmds)
	// if (check_access(cmd_list) != 0)
	// {
	// 	//free everything
	// 	exit(18);
	// }

	// check size list, set up pipes (if cmd is builtin they are forked as well, but might have no effect on the main shell p)
		//get size of lists
	nr_cmd = get_nr_cmd(cmd_list);
	printf("\n\nsize cmd list: %i\n\n", nr_cmd);
	// in each kid, redirect as needed

	// in each kid, exec



	//random to silence comp warning
	print_env(envp);
}