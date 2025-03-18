/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:31:13 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/18 15:39:21 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_only_digits(t_command *cmd)
{
	int	no_digit;
	int	i;

	i = 0;
	no_digit = 0;
	while (cmd->args[1][i])
	{
		if (ft_isdigit(cmd->args[1][i]) == 0)
			no_digit = 1;
		i++;
	}
	return (no_digit);
}

void	free_rest_exit(t_command *cmd, t_env *envp, int stat)
{
	if (envp)
		free_env_list(&envp);
	if (cmd)
		free_cmd_list(&cmd);
	rl_clear_history();
	exit(stat);
}

int	check_first_arg(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}
