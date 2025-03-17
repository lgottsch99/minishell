/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:24:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 15:13:01 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_exec_path(char *cmd, char **path)
{
	char	*tmp;
	char	*commandpath;

	while (*path)
	{
		tmp = ft_strjoin(*path, "/");
		commandpath = ft_strjoin(tmp, cmd);
		free(tmp);
		if ((access(commandpath, X_OK)) == 0)
			return (commandpath);
		free(commandpath);
		path++;
	}
	return (NULL);
}

void	search_path(t_command *cmd, t_env *envp)
{
	char	*fullpath;
	char	*exec_path;
	char	**paths;

	if (cmd && cmd ->args && cmd->args[0] && (cmd->args[0][0] == '\0'
		|| cmd->args[0][0] == '.' || cmd->args[0][0] == '/'))
		return ;
	fullpath = ret_value_env("PATH", envp);
	if (!fullpath)
		return ;
	paths = ft_split(fullpath, ':');
	if (cmd && cmd->args && cmd->args[0] && paths)
		exec_path = get_exec_path(cmd->args[0], paths);
	else
	{
		printf("Error: Invalid command arguments.\n");
		exec_path = NULL;
	}
	free_2d_char(paths);
	cmd->exec_path = exec_path;
}

void	check_path(t_command *cmd, t_env *envp)
{
	char	*dup;

	dup = NULL;
	if (cmd && cmd ->args && cmd->args[0] && cmd->args[0][0] == '/'
		&& cmd->args[0][1] != '/' && cmd->args[0][1] != '\0')
	{
		dup = ft_strdup(cmd->args[0]);
		cmd->exec_path = dup;
		return ;
	}
	if (cmd && cmd ->args && cmd->args[0] && cmd->args[0][0] == '.'
		&& cmd->args[0][1] == '/')
	{
		cmd->exec_path = extend_current_dir(cmd);
		return ;
	}
	if (cmd && cmd ->args && cmd->args[0] && cmd->args[0][0] == '.'
		&& cmd->args[0][1] == '.' && cmd->args[0][2] == '/')
	{
		cmd->exec_path = extend_upper_dir(cmd);
		return ;
	}
	else
		search_path(cmd, envp);
}

char	*create_fullstr(t_env *node)
{
	char	*equal;
	char	*joined;

	equal = ft_strjoin(node->key, "=");
	if (!equal)
		return (NULL);
	joined = ft_strjoin(equal, node->value);
	if (!joined)
		return (NULL);
	free(equal);
	return (joined);
}
