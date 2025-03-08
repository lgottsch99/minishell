/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:58:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/08 00:15:36 by Watanudon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Unsetting a variable that was not previously set shall not be  con‐
  //     sidered an error and does not cause the shell to abort.
  //should also work with multiple at once  //eg unset hello lilli world

static void	remove_from_envp(t_env *existing_var, t_env **envp)
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

int	unset(t_command *cmd, t_env *envp) //TO DO valgrind
{	
	int 	num_args;
	int		i;
	t_env	*existing_var;

	num_args = get_num_args(cmd->args);
	printf("nr unset args is: %i\n", num_args);
	i = 1; //skip "unset" itself
	while (cmd->args[i])//go thru cmd args (skip 1.)
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