/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:46:29 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/25 16:57:46 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_files(t_command *cmd, int *exit_stat)
{
	if (access(cmd->input_file, F_OK) == 0)
	{
		if (access(cmd->input_file, R_OK) == 0)
			return (0);
		else
		{
			printf("read access to %s denied\n", cmd->input_file);
			*exit_stat = 1;
			return (1);
		}
	}
	else
	{
		printf("%s does not exist\n", cmd->input_file);
		*exit_stat = 1;
		return (1);
	}
	return (0);
}

int	loop_check_access(t_command *tmp, t_env *envp, int *exit_stat)
{
	if (tmp->is_builtin == 0)
		check_path(tmp, envp);
	if (!tmp->exec_path && tmp->is_builtin == 0)
	{
		printf("minishell: cant find command\n");
		*exit_stat = 127;
		return (1);
	}
	if (tmp->is_builtin == 0
		&& (!tmp->exec_path || access(tmp->exec_path, X_OK) != 0))
	{
		printf("no executable path found\n");
		*exit_stat = 127;
		return (1);
	}
	if (tmp->input_file)
	{
		if (check_files(tmp, exit_stat) == 1)
			return (1);
	}
	return (0);
}
