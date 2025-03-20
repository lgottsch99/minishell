/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:47:57 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/20 19:43:30 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_env_oldpwd(char *cwd, t_env *envp)
{
	t_env	*oldpwd;

	oldpwd = check_existing_env("OLDPWD", envp);
	if (oldpwd)
	{
		free(oldpwd->value);
		oldpwd->value = ft_strdup(cwd);
	}
	if (cwd)
		free(cwd);
}

static void	update_env_pwd(t_env *envp)
{
	t_env	*pwd;
	char	*newcwd;

	pwd = check_existing_env("PWD", envp);
	if (pwd)
	{
		newcwd = NULL;
		newcwd = getcwd(newcwd, 0);
		if (!newcwd)
			return ;
		free(pwd->value);
		pwd->value = ft_strdup(newcwd);
		if (newcwd)
			free(newcwd);
	}
	return ;
}

static int	change_for_real(char *path, t_env *envp)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		perror("error cd: ");
		return (1);
	}
	if (chdir(path) == -1)
	{
		free(cwd);
		perror("cd: ");
		return (1);
	}
	update_env_oldpwd(cwd, envp);
	update_env_pwd(envp);
	return (0);
}

int	decide_more(t_command *cmd_list, t_env *envp)
{
	char	*old_pwd;
	int		stat;

	stat = 0;
	if (cmd_list->args[1][0] == '-')
	{
		old_pwd = ret_value_env("OLDPWD", envp);
		if (!old_pwd)
		{
			printf("cd error: cant find env var OLDPWD\n");
			return (1);
		}
		else
			stat = change_for_real(old_pwd, envp);
	}
	else
		stat = change_for_real(cmd_list->args[1], envp);
	return (stat);
}

int	decide_case(t_command *cmd_list, char *home, t_env *envp)
{
	int		stat;

	stat = 0;
	if (!cmd_list->args[1])
	{
		if (!home)
		{
			printf("cd error: cant find env var HOME\n");
			return (1);
		}
		else
			stat = change_for_real(home, envp);
	}
	else
		stat = decide_more(cmd_list, envp);
	return (stat);
}
