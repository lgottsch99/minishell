/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:07:51 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/11 20:35:48 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

static void	free_rest(t_command *cmd, t_env *envp)
{
	if (envp)
	free_env_list(&envp);
	if (cmd)
	free_cmd_list(&cmd);
}

static void	only_exit(t_command *cmd, t_env *envp, t_pipeline *pipeline, int *exit_stat)
{
	int stat;

	if (pipeline != NULL)
	{
		stat = *pipeline->exit_stat;
		free_everything_pipeline_exit(envp, pipeline, stat); //exit stat here is 1
	}
	else
	{
		stat = *exit_stat;
		free_rest(cmd, envp);
		exit(stat);
	}
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

// static int	exit_with_nr(t_command *cmd, t_env *envp, t_pipeline *pipeline)
// {
// 	int	stat;
// 	int no_digit;

// 	no_digit = check_only_digits(cmd);
// 	if (no_digit == 1)
// 	{
// 		printf("exit: numeric arg required\n");
// 		return (1);
// 	}
// 	stat = ft_atoi(cmd->args[1]);
// 	if (pipeline != NULL)
// 		free_everything_pipeline_exit(envp, pipeline, stat);
// 	else
// 	{
// 		free_rest(cmd, envp);
// 		exit(stat);
// 	}
// 	return 0;
// }

// int exit_shell(t_command *cmd, t_env *envp, t_pipeline *pipeline) //TODO double check valgrind
// {
// 	int	num_args;

// 	printf("exit\n"); //bash prints exit 
// 	num_args = get_num_args(cmd->args);
// 	if (num_args == 1)//only exit but no args OK
// 		only_exit(cmd, envp, pipeline);



// 	//TODO check bash behavior
	
	
// 	else if (num_args == 2) //if only one: exit with nr 
// 	{
// 		if (exit_with_nr(cmd, envp, pipeline) == 1)
// 			return 1;
// 	}
// 	else if (num_args > 2)//if more than one number: error mdg, set exit stat
// 	{
// 		printf("exit: too many arguments\n");
// 		return (1);
// 	}
// 	return (1);
// }

void	exit_one_arg(t_command *cmd, t_env *envp, t_pipeline *pipeline)
{//only one arg after exit
	int no_digit;
	int stat;

	stat = 0;
	no_digit = check_only_digits(cmd);
	if (no_digit == 1)//sth is not numeric in first
	{
		printf("exit: numeric argument required\n");
		if (pipeline != NULL)
			free_everything_pipeline_exit(envp, pipeline, 2);
		else
		{
			free_rest(cmd, envp);
			exit(2);
		}
	}
	else //only numeric -> atoi
	{
		stat = ft_atoi(cmd->args[1]);
		if (pipeline != NULL)
			free_everything_pipeline_exit(envp, pipeline, stat);
		else
		{
			free_rest(cmd, envp);
			exit(stat);
		}
	}
}

int	check_first_arg(char *arg) //ret 1 is sth else than digit found
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return 1;
		i++;
	}
	return 0;
}

void	many_args(t_command *cmd, t_env *envp, t_pipeline *pipeline)
{
	int no_digit;

	no_digit = check_first_arg(cmd->args[1]);
	if (no_digit == 1)
	{//if first is NOT numeric:
		printf("exit: numeric argument required\n");
		
		if (pipeline != NULL)
			free_everything_pipeline_exit(envp, pipeline, 2);
		else
		{
			free_rest(cmd, envp);
			exit(2);
		}	
	}
	//if first IS num:
	else
	{
		printf("exit: too many arguments\n");
		//DO exit
		// if (pipeline != NULL)
		// free_everything_pipeline_exit(envp, pipeline, 2);
		// else
		// {
		// 	free_rest(cmd, envp);
		// 	exit(2);
		// }
		return ;
	}
}

int		exit_shell(t_command *cmd, t_env *envp, t_pipeline *pipeline, int *exit_stat)
{
	int	num_args;

	printf("exit\n"); //bash prints exit 
	num_args = get_num_args(cmd->args);
	
	if (num_args == 1)//only exit 
		only_exit(cmd, envp, pipeline, exit_stat);
	else if (num_args == 2)
		exit_one_arg(cmd, envp, pipeline);
	else //more than one arg 
	{
		many_args(cmd, envp, pipeline);

		return (1);
	}
	//TODO check bash behavior
	return (0);
}
