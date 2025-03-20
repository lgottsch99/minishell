/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:37:50 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/20 18:36:19 by lgottsch         ###   ########.fr       */
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

int	change_for_real(char *path, t_env *envp) //TODO only update PWD and OLDPWD
{
	char	*cwd;
	char	*newcwd;
	t_env	*oldpwd;
	t_env	*pwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0); //MALLOC
	if (!cwd)
	{
		perror("error cd: ");
		return (1);
	}
	printf("change from: %s\n", cwd);
	if (chdir(path) == -1)
	{
		free(cwd);
		perror("cd: ");
		return (1);
	}
	//TODO set cwd -> OLDPWD
		//check if OLDPWD existing
	oldpwd = check_existing_env("OLDPWD", envp);
	if (oldpwd)
	{
		printf("old oldpwd: %s\n",oldpwd->value);
		free(oldpwd->value);
		oldpwd->value = ft_strdup(cwd);
		printf("updated oldpwd : %s\n", oldpwd->value);
	}
	if (cwd)
		free(cwd);
	//TODO set PWD to getcwd
	pwd = check_existing_env("PWD", envp);
	if (pwd)
	{
		newcwd = NULL;
		newcwd = getcwd(newcwd, 0);
		if (!newcwd)
		{
			perror("error cd: ");
			return (1);
		}
		printf("change to: %s\n", newcwd);
		printf("key node: %s\n", pwd->key);
		free(pwd->value);
		pwd->value = ft_strdup(newcwd);
		printf("updated PWD: %s\n", pwd->value);
		if(newcwd)
			free(newcwd);
	}
	return (0);
}

static int	change_dir(t_command *cmd_list, char *home, t_env *envp)
{
	int	stat;
	char	*old_pwd;

	old_pwd = NULL;
	stat = 0;
	if (!cmd_list->args[1])
	{
		if (!home)
		{
			printf("cd error: cant find env var HOME\n");
			return (1);
		}
		else
		{
			stat = change_for_real(home, envp);
			//TODO set pwd -> OLDPWD
			// if (chdir(home) == -1)
			// {
			// 	perror("cd: ");
			// 	return (1);
			// }
			//TODO set PWD to getcwd
		}
	}
	else if (cmd_list->args[1])
	{
		//to do check if args[1] == -
		if (cmd_list->args[1][0] == '-')
		{
			//then get oldpwd
			old_pwd = ret_value_env("OLDPWD", envp);
			if (!old_pwd)
			{
				printf("cd error: cant find env var OLDPWD\n");
				return (1);		
			}
			else
			{
				stat = change_for_real(old_pwd, envp);
				// //TODO set pwd -> OLDPWD
				// if (chdir(old_pwd) == -1)
				// {
				// 	perror("cd: ");
				// 	return (1);
				// }
				// //TODO set PWD to getcwd
			}
		}

		else
		{
			stat = change_for_real(cmd_list->args[1], envp);

			// //TODO set pwd -> OLDPWD
			// if (chdir(cmd_list->args[1]) == -1)
			// {
			// 	perror("cd: ");
			// 	return (1);
			// }
			// //TODO set PWD to getcwd
		}
	}
	return (stat);
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
		if (change_dir(cmd_list, home, envp) == 1)
			return (1);
	}
	return (0);
}
