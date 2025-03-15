/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:46:29 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/15 16:12:34 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_files(t_command *cmd, int *exit_stat) //ret 1 if denied, 0 if ok
{
	printf("in check files\n");
	
	//read access infile
	if (access(cmd->input_file, F_OK) == 0) //file 1 need to exist
	{
		if (access(cmd->input_file, R_OK) == 0) //read access ok
			return (0);
		else 
		{
			printf("read access to %s denied\n", cmd->input_file);
			*exit_stat = 1;
			return (1);
		}
	}
	else 
	{
		printf("%s does not exist\n", cmd->input_file);
		*exit_stat = 1;
		return (1);
	}
	return (0);
}


int	loop_check_access(t_command *tmp,t_env *envp, int *exit_stat)
{
	if (tmp->is_builtin == 0)
		check_path(tmp, envp);
	else if (tmp->is_builtin == 1 && tmp->args[0]) //remove
		printf("cmd is builtin: %s\n", tmp->args[0]);//remove

	if (!tmp->exec_path && tmp->is_builtin == 0) //cmd not found
	{
		printf("minishell error: cant find command\n");
		*exit_stat = 127;
		return (1);
	}
	if (tmp->is_builtin == 0 && (!tmp->exec_path || access(tmp->exec_path, X_OK) != 0))//check executability of path
	{
		printf("no executable path found\n");
		*exit_stat = 127;
		return (1);
	}
	//check access to files
	if (tmp->input_file)
	{
		if (check_files(tmp, exit_stat) == 1)			
			return (1);
	}
	return (0);
}


int	check_access(t_command	*cmd_list, int nr_cmd, t_env *envp, int *exit_stat)//ret 1 if access denied, 0 if ok
{
	printf("in check access \n");//remove
	
	int			i;
	t_command	*tmp; //to trav list

	tmp = cmd_list;
	i = 0;
	while (i < nr_cmd)	//for each cmd in list
	{
		if (loop_check_access(tmp, envp, exit_stat) == 1)
			return (1);

		tmp = tmp->next;
		i++;
	}
	return (0);
}

