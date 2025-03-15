/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:24:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/15 16:47:23 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_exec_path(char *cmd, char **path)
{
	printf("in get exec path \n");
    char *tmp;
    char *commandpath; //zb /usr/bin/ls

    while (*path)
    {
		//printf("in loop \n");
        tmp = ft_strjoin(*path, "/"); //MALLOC
        commandpath = ft_strjoin(tmp, cmd); //MALLOC
        free(tmp);
        if ((access(commandpath, X_OK)) == 0)
            return (commandpath);
        free(commandpath);
        path++;
    }
    return(NULL);
}


// char	*extend_upper_dir(t_command *cmd) // ../
// {
// 	char	*cwd;
// 	char	*sub;
// 	char	*joined;
// 	int		i;
// 	char	*cutpath;

// 	cwd = NULL;
// 	cwd = getcwd(cwd, PATH_MAX);
// 	if (!cwd)
// 	{
// 	 	perror("error getting working dir: ");
// 		//free everything 
// 		return (NULL);
// 	}
// 	printf("cwd: %s\n", cwd); //remove
// 	//count to last / and cut off in cwd
// 	i = ft_strlen(cwd) - 1; //-1?
// 	printf("i: %i\n", i);//remove

// 	while (cwd[i] != '/')
// 		i--;
	
// 	//substr path 
// 	cutpath = ft_substr(cwd, 0, i);
// 	if (! cutpath)
// 		return NULL;
// 	printf("cutpath: %s\n", cutpath);//remove
// 	//substr arg - 2
// 	sub = ft_substr(cmd->args[0], 2, ft_strlen(cmd->args[0]) - 2);
// 	if (!sub)
// 		return (NULL);
// 	printf("sub: %s\n", sub);//remove
// 	//strjoin
// 	joined = ft_strjoin(cutpath, sub);
// 	printf("joined: %s\n", joined);

// 	free(cwd);
// 	free(cutpath);
// 	free(sub);
// 	return (joined);
// }

// char	*extend_current_dir(t_command *cmd) // ./
// { //construct a fuull path with appending cmd arg[o] to cwd
// 	char	*cwd;
// 	char	*sub;
// 	char	*joined;

// 	cwd = NULL;
// 	cwd = getcwd(cwd, PATH_MAX);
// 	if (!cwd)
// 	{
// 	 	perror("error getting working dir: ");
// 		//free everything 
// 		return (NULL);
// 	}
// 	printf("cwd: %s\n", cwd);

// 	//substr to get rid of .
// 	sub = ft_substr(cmd->args[0], 1, ft_strlen(cmd->args[0]) - 1);
// 	if (!sub)
// 		return (NULL);

// 	printf("sub: %s\n", sub);
// 	//strjoin
// 	joined = ft_strjoin(cwd, sub);
// 	free(cwd);
// 	free(sub);
// 	return (joined);
// }



// int	check_absolute(t_command *cmd)		//EVEN USED ANYWHERE????????
// {	//1. check if absolute path (no . or .. and starting with /
// 		//rules: start with / and only / or alphanumeric
// 	int		i;
// 	char	*dup;

// 	dup = NULL;
// 	i = 0;
// 	if (cmd->args[0][0] == '/')
// 	{
// 		// printf("first is /\n");
// 		//go thru whole str and check if rest only / and alphanumeric
// 		while (cmd->args[0][i])
// 		{
// 			// printf("in loop\n");
// 			if (cmd->args[0][i] == '/' || ft_isalnum(cmd->args[0][i]) == 1)
// 				i++;
// 			else
// 				break;
// 		}
// 		// printf("strlen cmd arg 0: %i\n", (int)ft_strlen(cmd->args[0]));
// 		// printf("i from loop: %i\n", i);
// 		if ((int)ft_strlen(cmd->args[0]) == i)//went thru whole str all absolute
// 		{
// 			dup = ft_strdup(cmd->args[0]);
// 			if (!dup)
// 				return (1);
// 			cmd->exec_path = dup;
// 			return (0);
// 		}
// 	}
// 	return (1);
// }


void	search_path(t_command *cmd, t_env *envp)
{
	char	*fullpath; //whole path from envp
	char	*exec_path; //path that can be exec
	char	**paths;

	if (cmd && cmd ->args && cmd->args[0] && (cmd->args[0][0] == '\0' || cmd->args[0][0] == '.' || cmd->args[0][0] == '/'))
		return ;
	//1. get whole path from env
	fullpath = ret_value_env("PATH", envp);

	if (!fullpath) //env PATH was unset
		return;
	printf("fullpath: %s\n", fullpath);
	paths = ft_split(fullpath, ':');
	//2. get executable path if cmd not builtin
	if (cmd && cmd->args && cmd->args[0] && paths)
		exec_path = get_exec_path(cmd->args[0], paths); //returns malloced str if exists, NULL if not
	else
	{
		printf("Error: Invalid command arguments.\n");
		exec_path = NULL; // Handle error appropriately
	}
	free_2d_char(paths);
	//3. save exec path in cmd table
	cmd->exec_path = exec_path;
}


void	check_path(t_command *cmd, t_env *envp) //TODO
{printf("in check path \n");
	char	*dup;

	dup = NULL;
	if (cmd && cmd ->args && cmd->args[0] && cmd->args[0][0] == '/' && cmd->args[0][1] != '/' && cmd->args[0][1] != '\0')
	{ printf("absolute/ \n");
		dup = ft_strdup(cmd->args[0]);
		cmd->exec_path = dup;
		return;
	}
	if (cmd && cmd ->args && cmd->args[0] && cmd->args[0][0] == '.' && cmd->args[0][1] == '/') // ./
	{	printf("extending ./ \n");
		cmd->exec_path = extend_current_dir(cmd);
		printf("exec path: %s\n", cmd->exec_path);
		return;
	}
	//0. check if ./ or ../
	if (cmd && cmd ->args && cmd->args[0] && cmd->args[0][0] == '.' && cmd->args[0][1] == '.' && cmd->args[0][2] == '/') // ../
	{		printf("extending ../ \n");
		cmd->exec_path = extend_upper_dir(cmd);
		return;
	}
	else
		search_path(cmd, envp);
}




char	*create_fullstr(t_env *node) //MALLOC
{
	// int		fullsize;
	char	*equal;
	char	*joined;
	// int		i;

	// printf("key: %s\n", (char *)node->key);
	// printf("value: %s\n", (char *)node->value);

	equal = ft_strjoin(node->key, "="); //MLLOC
	if(!equal)
		return NULL;
	// printf("key + =: %s\n", equal);
	joined = ft_strjoin(equal, node->value);//MLLOC
	if(!joined)
		return NULL;
	// printf("full: %s\n", joined);

	free(equal);
	return (joined);
}

