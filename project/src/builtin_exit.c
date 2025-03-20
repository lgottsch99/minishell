/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:07:51 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/18 18:28:12 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	only_exit(t_command *cmd, t_env *envp,
	t_pipeline *pipeline, int *exit_stat)
{
	int	stat;

	if (pipeline != NULL)
	{
		stat = *pipeline->exit_stat;
		remove_heredoc(cmd->heredoc_file);
		free_everything_pipeline_exit(envp, pipeline, stat % 256);
	}
	else
	{
		stat = *exit_stat;
		free_rest_exit(cmd, envp, stat % 256);
	}
}

static void	exit_one_arg(t_command *cmd, t_env *envp, t_pipeline *pipeline)
{
	int	no_digit;
	int	stat;

	stat = 0;
	no_digit = check_only_digits(cmd);
	if (no_digit == 1)
	{
		printf("exit: numeric argument required\n");
		if (pipeline != NULL)
			free_everything_pipeline_exit(envp, pipeline, 2);
		else
			free_rest_exit(cmd, envp, 2);
	}
	else
	{
		stat = ft_atoi(cmd->args[1]);
		if (pipeline != NULL)
			free_everything_pipeline_exit(envp, pipeline, stat % 256);
		else
			free_rest_exit(cmd, envp, stat % 256);
	}
}

static void	many_args(t_command *cmd, t_env *envp, t_pipeline *pipeline)
{
	int	no_digit;

	no_digit = check_first_arg(cmd->args[1]);
	if (no_digit == 1)
	{
		printf("exit: numeric argument required\n");
		if (pipeline != NULL)
			free_everything_pipeline_exit(envp, pipeline, 2);
		else
			free_rest_exit(cmd, envp, 2);
	}
	else
	{
		printf("exit: too many arguments\n");
		return ;
	}
}

int	exit_shell(t_command *cmd, t_env *envp,
	t_pipeline *pipeline, int *exit_stat)
{
	int	num_args;

	printf("exit\n");
	num_args = get_num_args(cmd->args);
	if (num_args == 1)
		only_exit(cmd, envp, pipeline, exit_stat);
	else if (num_args == 2)
		exit_one_arg(cmd, envp, pipeline);
	else
	{
		many_args(cmd, envp, pipeline);
		return (1);
	}
	return (1);
}
