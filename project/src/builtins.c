/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/18 15:39:17 by lgottsch         ###   ########.fr       */
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

static int	change_dir(t_command *cmd_list, char *home)
{
	if (cmd_list && cmd_list->args)
	{
		if (!cmd_list->args[1] && home)
		{
			if (chdir(home) == -1)
			{
				perror("cd: ");
				return (1);
			}
		}
		else if (cmd_list->args[1])
		{
			if (chdir(cmd_list->args[1]) == -1)
			{
				perror("cd: ");
				return (1);
			}
		}
		return (0);
	}
	else
		return (1);
}

int	cd(t_command *cmd_list, t_env *envp)
{
	char	*home;
	int		num_args;

	home = ret_value_env("HOME", envp);
	if (!home)
	{
		printf("cd error: cant find env var HOME\n");
		return (1);
	}
	num_args = get_num_args(cmd_list->args);
	if (num_args > 2)
	{
		printf("cd: too many args\n");
		return (1);
	}
	if (change_dir(cmd_list, home) == 1)
		return (1);
	return (0);
}
