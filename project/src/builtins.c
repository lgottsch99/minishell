/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/20 19:28:52 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	run_builtin(t_command *cmd_list, t_env *envp, t_pipeline *pipeline,
	int *exit_stat)
{
	if (ft_strncmp(cmd_list->args[0], "env", ft_strlen(cmd_list->args[0])) == 0)
		*exit_stat = print_env(envp);
	else if (ft_strncmp(cmd_list->args[0], "echo",
			ft_strlen(cmd_list->args[0])) == 0)
		*exit_stat = echo(cmd_list);
	else if (ft_strncmp(cmd_list->args[0], "pwd",
			ft_strlen(cmd_list->args[0])) == 0)
		*exit_stat = pwd();
	else if (ft_strncmp(cmd_list->args[0], "exit",
			ft_strlen(cmd_list->args[0])) == 0)
		*exit_stat = exit_shell(cmd_list, envp, pipeline, exit_stat);
	else if (ft_strncmp(cmd_list->args[0], "cd",
			ft_strlen(cmd_list->args[0])) == 0)
		*exit_stat = cd(cmd_list, envp);
	else if (ft_strncmp(cmd_list->args[0], "export",
			ft_strlen(cmd_list->args[0])) == 0)
		*exit_stat = eexport(cmd_list, envp);
	else if (ft_strncmp(cmd_list->args[0], "unset",
			ft_strlen(cmd_list->args[0])) == 0)
		*exit_stat = unset(cmd_list, envp);
}

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

int	pwd(void)
{
	char	*path;

	path = NULL;
	path = getcwd(path, 0);
	if (!path)
	{
		perror("error pwd: ");
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}

int	cd(t_command *cmd_list, t_env *envp)
{
	char	*home;
	int		num_args;

	home = ret_value_env("HOME", envp);
	num_args = get_num_args(cmd_list->args);
	if (num_args > 2)
	{
		printf("cd: too many args\n");
		return (1);
	}
	if (cmd_list && cmd_list->args)
	{
		if (decide_case(cmd_list, home, envp) == 1)
			return (1);
	}
	return (0);
}
