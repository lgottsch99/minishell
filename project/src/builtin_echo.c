/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:29:09 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/18 20:00:10 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_num_args(char **args)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = args;
	while (tmp[i])
		i++;
	return (i);
}

static int	print_args(char **args, int start)
{
	while (args[start])
	{
		if (args[start][0] == '\0')
		{
			start++;
			continue ;
		}
		if (printf("%s", args[start]) < 0)
			return (1);
		if (args[start + 1])
		{
			if (printf(" ") < 0)
				return (1);
		}
		start++;
	}
	return (0);
}

int	echo(t_command *cmd_list)
{
	int	nl;
	int	start_arg;
	int	y;

	nl = 1;
	start_arg = 1;
	while (cmd_list->args[start_arg]
		&& ft_strncmp(cmd_list->args[start_arg], "-n", 2) == 0)
	{
		y = 2;
		while (cmd_list->args[start_arg][y] == 'n')
			y++;
		if (cmd_list->args[start_arg][y] != '\0')
			break ;
		nl = 0;
		start_arg++;
	}
	if (print_args(cmd_list->args, start_arg) == 1)
		return (1);
	if (nl == 1)
	{
		if (printf("\n") < 0)
			return (1);
	}
	return (0);
}
