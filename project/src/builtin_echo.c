/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:29:09 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/08 17:07:07 by lgottsch         ###   ########.fr       */
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
	printf("num args is: %i\n", i);
	return (i);
}

int	print_args(char **args, int start)
{
	int		i;
	char **tmp;

	i = start;
	tmp = args;
	while (tmp[i])
	{
		if (printf("%s", tmp[i]) < 0)
			return (1);
		if (printf(" ") < 0)
			return (1);
		i++;
	}
	return (0);
}

static int echo_with_n(t_command *cmd_list, int num_args)
{
	int skip;

	skip = 2;
	while (cmd_list->args[skip] && (ft_strncmp(cmd_list->args[skip], "-n", ft_strlen(cmd_list->args[skip])) == 0))
		skip++;
	//print all other args starting at indx 2
	if (num_args > skip)
		if (print_args(cmd_list->args, skip) == 1)
			return (1);
	//NO NEW LINE AT END
	return (0);
}

int	echo(t_command *cmd_list)
{
	//printf("in echo\n");
	int num_args;

	num_args = get_num_args(cmd_list->args);
	if (num_args <= 1) //only echo
	{
		printf("only echo\n");
		if (printf("\n") < 0)
			return (1);
	}
	else if (cmd_list->args[1] &&cmd_list->args[1][0])
	{
		printf("not only echo\n");

		if (cmd_list->args[1][0] != '\0')
		{

	//check if -n or not 
		if (ft_strncmp(cmd_list->args[1], "-n", ft_strlen(cmd_list->args[1])) == 0)
		{
			printf(" echo with -n\n");

			if (echo_with_n(cmd_list, num_args) == 1)
				return (1);
		}
		else //print all other args starting at indx 1
		{
			if (print_args(cmd_list->args, 1) == 1)
				return (1);
			if (printf("\n") < 0) //INCL NL
				return (1);
		}
	}
}
	// }
	return (0);
}

