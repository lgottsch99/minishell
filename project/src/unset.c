/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:58:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/25 12:40:04 by Watanudon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Unsetting a variable that was not previously set shall not be  con‐
  //     sidered an error and does not cause the shell to abort.
  //should also work with multiple at once  //eg unset hello lilli world

// int	check_unset_rules(t_command *cmd) //check if any char in char **args that is not a 
// {
// 	//go thru each arg 
// 	//check if start  not a-z or '_' (NO DIGITS ALLOWED)
// 	//goo thru whole arg and check if anything not a-z or 0-9 or _ (NO =)
// 	int	i;
// 	int	y;
	
// 	i = 0;
// 	y = 0;
// 	while (cmd->args[i])
// 	{
// 		//check char[0]: if not a-z or '_' (NO DIGITS)
// 		if (ft_isalpha(cmd->args[i][0]) == 0 && cmd->args[i][0] != '_')
// 			return (1);
// 		//goo thru whole arg and check if anything not a-z or 0-9 or _
// 		y = 1;
// 		while (cmd->args[i][y])
// 		{
// 			if (ft_isalnum(cmd->args[i][y]) == 0 && cmd->args[i][y] != '_')
// 				return (1);
// 			y++;
// 		}
// 		i++;
// 	}
// 	return (0);
// }

void	remove_from_envp(t_env *existing_var, t_env **envp)
{
	printf("in remove_from_envp\n");

	t_env	*before;
	int		lstsize;

	lstsize = count_env_size(*envp);
	printf("envp size: %i\n", lstsize);

	//get node before if exists else = envp
	before = *envp;
	if (before->next)
	{
		while (ft_strncmp((char *)before->next->key, existing_var->key, ft_strlen(existing_var->key)) != 0)
			before = before->next;
	}
	//connect node before to node after if exists, else node before ->next = NULL
	if (before->next->next)
		before->next = before->next->next;
	else
		before->next = NULL;
	//free existing var
	free(existing_var->key); //CHECK IF CORRECT
	free(existing_var->value); //CHECK IF CORRECT
	free(existing_var);

}

int	unset(t_command *cmd, t_env *envp)
{	
	int 	num_args;
	int		i;
	t_env	*existing_var;

	// if (check_unset_rules(cmd) == 1) //1 if invalid, 0 if valid //TODO NoT REALLY NEEDED ->bash
	// {
	// 	ft_putstr_fd("invalid shell var format\n", 1);//print error
	// 	//free and exit
	// 	exit(34404);
	// }

	num_args = get_num_args(cmd->args);
	printf("nr unset args is: %i\n", num_args);
	// if (num_args == 1) //only unset
	// {
	// 	ft_putstr_fd("unset error: not enough arguments\n", 1);//print error
	// 	//free and exit
	// 	exit(34404);
	// }
	i = 1; //skip "unset"
	//go thru cmd args (skip 1.)
	while (cmd->args[i])
	{
		//for each:
	//check if var to unset exists
		existing_var = check_existing_env(cmd->args[i], envp);
		if (existing_var)
		{ 	//if yes remove from list
			remove_from_envp(existing_var, &envp);

		}//if no do nothing but dont exit shell
		i++;
	}
	return (0);
}