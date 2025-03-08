//HEADER

#include "../includes/minishell.h"

static void	free_rest(t_command *cmd, t_env *envp)
{
	if (envp)
	free_env_list(&envp);
	if (cmd)
	free_cmd_list(&cmd);
}

static void	only_exit(t_command *cmd, t_env *envp, t_pipeline *pipeline)
{
	if (pipeline != NULL)
		free_everything_pipeline_exit(envp, pipeline, 1); //exit stat here is 1
	else
		free_rest(cmd, envp);
	exit(0);
}

static int	check_only_digits(t_command *cmd)
{
	int	no_digit;
	int	i;

	i = 0;
	no_digit = 0;
	while (cmd->args[1][i])
	{
		if (ft_isdigit(cmd->args[1][i]) == 0) //check if arg nr is only digits 
			no_digit = 1;
		i++;
	}
	return (no_digit);
}

static int	exit_with_nr(t_command *cmd, t_env *envp, t_pipeline *pipeline)
{
	int	stat;
	int no_digit;

	no_digit = check_only_digits(cmd);
	if (no_digit == 1)
	{
		printf("exit: numeric arg required\n");
		return (1);
	}
	stat = ft_atoi(cmd->args[1]);
	if (pipeline != NULL)
		free_everything_pipeline_exit(envp, pipeline, stat);
	else
	{
		free_rest(cmd, envp);
		exit(stat);
	}
	return 0;
}

int exit_shell(t_command *cmd, t_env *envp, t_pipeline *pipeline) //TODO double check valgrind
{
	int	num_args;

	printf("exit\n"); //bash prints exit 
	num_args = get_num_args(cmd->args);
	if (num_args == 1)//only exit but no args
		only_exit(cmd, envp, pipeline);
	else if (num_args == 2) //if only one: exit with nr 
	{
		if (exit_with_nr(cmd, envp, pipeline) == 1)
			return 1;
	}
	else if (num_args > 2)//if more than one number: error mdg, set exit stat
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	return (1);
}
