/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:36:59 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 13:49:50 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_value_only(char *arg)
{
	int		index;
	char	*value;

	index = 0;
	while (arg[index] != '=')
		index++;
	value = ft_substr(arg, index + 1, ft_strlen(arg) - index);
	if (!value)
		return (NULL);
	printf("new value: %s\n", value);
	return (value);
}

char	*get_name_only(char *str)
{
	int	index;
	int	len;

	len = ft_strlen(str);
	index = 0;
	while (str[index])
	{
		if (str[index] == '=')
			break ;
		index++;
	}
	if (index < len)
		return (ft_substr(str, 0, index));
	if (index == len)
		return (ft_substr(str, 0, len));
	return (NULL);
}

//SHELL VAR RULES:  A variable name could contain any alphabet 
//(a-z, A-Z), any digits (0-9), and an underscore ( _ ). However, 
// a variable name must start with an alphabet 
// or underscore. It can never start with a number. 
int	check_shellvar_rules(t_command *cmd)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while (cmd->args[i])
	{
		if (ft_isalpha(cmd->args[i][0]) == 0 && cmd->args[i][0] != '_')
			return (1);
		y = 1;
		while (cmd->args[i][y])
		{
			if (ft_isalnum(cmd->args[i][y]) == 0 && cmd->args[i][y] != '_'
			&& cmd->args[i][y] != '=' && cmd->args[i][y] != '"')
				return (1);
			y++;
		}
		i++;
	}
	return (0);
}
