/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/08 12:33:58 by Watanudon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
//env list struct
typedef struct s_env {
	char			*key; //eg PATH
	char			*value; // /home/lgottsch/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin
	struct s_env	*next;
} t_env;
*/

#include "../includes/minishell.h"

int	print_env(t_env *environ)
{
	t_env	*tmp;

	tmp = environ;
	while (tmp)
	{
		if (printf("%s", (char *)tmp->key) < 0)
			return (1);
		if (printf("=") < 0)
			return (1);
		if (printf("%s\n", (char *)tmp->value) < 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	pwd(void) //test with very long path TODO
{//if pwd hjfdh sdj  also works just like pwd ( doesnt care about args)
	char *path = NULL;

	path = getcwd(path, 0);
	if (!path)
	{
	 	perror("error pwd: ");
		//free everything 
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}

static int change_dir(t_command *cmd_list, char *home)
{
	if (!cmd_list->args[1] && home) //only cd -> goes to home //OK
	{
		if (chdir(home) == -1)
		{
			perror("cd: ");
			return (1);
		}
	}
	else //cd fhksh/gdf/ -> goes to that dir
	{
		if (chdir(cmd_list->args[1]) == -1)
		{
			perror("cd: ");
			return (1);
		}
	}
	return (0);
}

int	cd(t_command *cmd_list, t_env *envp)
{
	char s[100];//remove
	printf("in cd\n");//remove
	printf("current dir: %s\n", getcwd(s, 100));//remove

	char	*home;
	int		num_args;

	home = ret_value_env("HOME", envp);
	if (!home)
	{
		printf("cant find home\n");
		return (1);
	}
	num_args = get_num_args(cmd_list->args);
	if (num_args > 2) //OK
	{	
		printf("cd: too many args\n");
		return(1);
	}
	if (change_dir(cmd_list, home) == 1)
		return (1);

	printf("new dir: %s\n", getcwd(s, 100)); //remove
	return (0);
}

// void	print_list(t_list *envp) //even used???
// {
// 	t_list	*tmp;

// 	tmp = envp;
// 	while(tmp)
// 	{
// 		printf("%s\n", (char *)tmp->content);
// 		tmp = tmp->next;
// 	}
// }

