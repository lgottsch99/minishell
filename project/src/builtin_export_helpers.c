/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:36:59 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/06 15:44:40 by lgottsch         ###   ########.fr       */
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
// int	check_shellvar_rules(t_command *cmd)
int	check_shellvar_rules(char *arg)
{
	int	y;

	y = 0;
	if (ft_isalpha(arg[0]) == 0 && arg[0] != '_')
		return (1);
	y = 1;
	while (arg[y])
	{
		if (ft_isalnum(arg[y]) == 0 && arg[y] != '_'
			&& arg[y] != '=' && arg[y] != '"')
			return (1);
		y++;
	}
	return (0);
}
