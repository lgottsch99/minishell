/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:01:17 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/15 16:20:04 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*create_joined(char *cwd, int i, t_command *cmd)
{
	char	*sub;
	char	*joined;
	char	*cutpath;

	//substr path 
	cutpath = ft_substr(cwd, 0, i);
	if (! cutpath)
		return NULL;
	printf("cutpath: %s\n", cutpath);//remove
	//substr arg - 2
	sub = ft_substr(cmd->args[0], 2, ft_strlen(cmd->args[0]) - 2);
	if (!sub)
		return (NULL);
	printf("sub: %s\n", sub);//remove
	//strjoin
	joined = ft_strjoin(cutpath, sub);
	printf("joined: %s\n", joined);
	
	free(cwd);
	free(cutpath);
	free(sub);
	return  (joined);
}

char	*extend_upper_dir(t_command *cmd) // ../
{
	char	*cwd;
	char	*joined;
	int		i;

	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (!cwd)
	{
	 	perror("error getting working dir: ");
		//free everything 
		return (NULL);
	}
	printf("cwd: %s\n", cwd); //remove
	//count to last / and cut off in cwd
	i = ft_strlen(cwd) - 1; //-1?
	printf("i: %i\n", i);//remove

	while (cwd[i] != '/')
		i--;
	
	joined = create_joined(cwd, i, cmd);

	return (joined);
}


char	*extend_current_dir(t_command *cmd) // ./
{ //construct a fuull path with appending cmd arg[o] to cwd
	char	*cwd;
	char	*sub;
	char	*joined;

	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (!cwd)
	{
	 	perror("error getting working dir: ");
		//free everything 
		return (NULL);
	}
	printf("cwd: %s\n", cwd); //remove

	//substr to get rid of .
	sub = ft_substr(cmd->args[0], 1, ft_strlen(cmd->args[0]) - 1);
	if (!sub)
		return (NULL);

	printf("sub: %s\n", sub); //remove
	//strjoin
	joined = ft_strjoin(cwd, sub);
	free(cwd);
	free(sub);
	return (joined);
}
