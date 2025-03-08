//HEADER

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
	while (ft_strncmp(cmd_list->args[skip], "-n", ft_strlen(cmd_list->args[skip])) == 0)
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
		if (printf("\n") < 0)
			return (1);
	}
	else
	{	//check if -n or not 
		if (ft_strncmp(cmd_list->args[1], "-n", ft_strlen(cmd_list->args[1])) == 0)
		{
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
	return (0);
}

