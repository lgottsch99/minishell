/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:01:17 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/18 15:39:08 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*create_joined(char *cwd, int i, t_command *cmd)
{
	char	*sub;
	char	*joined;
	char	*cutpath;

	cutpath = ft_substr(cwd, 0, i);
	if (! cutpath)
		return (NULL);
	sub = ft_substr(cmd->args[0], 2, ft_strlen(cmd->args[0]) - 2);
	if (!sub)
		return (NULL);
	joined = ft_strjoin(cutpath, sub);
	free(cwd);
	free(cutpath);
	free(sub);
	return (joined);
}

char	*extend_upper_dir(t_command *cmd)
{
	char	*cwd;
	char	*joined;
	int		i;

	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (!cwd)
	{
		perror("error getting working dir: ");
		return (NULL);
	}
	i = ft_strlen(cwd) - 1;
	while (cwd[i] != '/')
		i--;
	joined = create_joined(cwd, i, cmd);
	return (joined);
}

char	*extend_current_dir(t_command *cmd)
{
	char	*cwd;
	char	*sub;
	char	*joined;

	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (!cwd)
	{
		perror("error getting working dir: ");
		return (NULL);
	}
	sub = ft_substr(cmd->args[0], 1, ft_strlen(cmd->args[0]) - 1);
	if (!sub)
		return (NULL);
	joined = ft_strjoin(cwd, sub);
	free(cwd);
	free(sub);
	return (joined);
}
