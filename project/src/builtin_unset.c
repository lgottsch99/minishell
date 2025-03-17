/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:58:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 14:13:24 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Unsetting a variable that was not previously set shall not be  con‐
//     sidered an error and does not cause the shell to abort.
//should also work with multiple at once  //eg unset hello lilli world
static void	remove_from_envp(t_env *existing_var, t_env **envp)
{
	t_env	*before;
	int		lstsize;

	lstsize = count_env_size(*envp);
	before = *envp;
	if (before->next)
	{
		while (ft_strncmp((char *)before->next->key, existing_var->key,
				ft_strlen(existing_var->key)) != 0)
			before = before->next;
	}
	if (before->next->next)
		before->next = before->next->next;
	else
		before->next = NULL;
	free(existing_var->key);
	free(existing_var->value);
	free(existing_var);
}

int	unset(t_command *cmd, t_env *envp)
{
	int		num_args;
	int		i;
	t_env	*existing_var;

	num_args = get_num_args(cmd->args);
	i = 1;
	while (cmd->args[i])
	{
		existing_var = check_existing_env(cmd->args[i], envp);
		if (existing_var)
			remove_from_envp(existing_var, &envp);
		i++;
	}
	return (0);
}
