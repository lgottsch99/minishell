/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:24:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/25 14:39:47 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_exec_path(char *cmd, char **path)
{
	//printf("in get exec path \n");

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





// int	check_access(t_command	*cmd_list, int nr_cmd, t_env *envp)//ret 1 if access denied, 0 if ok
// {
// 	//printf("in check access \n");

// 	int			i;
// //	int			builtin;
// 	t_command	*tmp; //to trav list

// 	tmp = cmd_list;
// 	i = 0;
// 	while (i < nr_cmd)	//for each cmd in list
// 	{
// 		//builtin = 0;
// 		//CHECK IF CMD IS BUILTIN
// 		//builtin = check_builtin(tmp->command);
// 		//IF NOT BUILTIN:
// 		if (tmp->is_builtin == 0)
// 			check_path(tmp, envp);
// 		else if (tmp->is_builtin == 1 && tmp->args[0])
// 			//printf("cmd is builtin: %s\n", tmp->args[0]);

// 		if (!tmp->exec_path && tmp->is_builtin == 0) //cmd not found
// 		{
// 			printf("minishell error: cant find command\n");
// 			return (1);
// 		}
// 		if (tmp->is_builtin == 0 && (!tmp->exec_path || access(tmp->exec_path, X_OK) != 0))//check executability of path
// 		{
// 			printf("no executable path found\n");
// 			return (1);
// 		}

// 		//check access to files
// 		if (tmp->input_file)
// 		{
// 			if (check_files(tmp) == 1)
// 				return (1);
// 		}
// 		tmp = tmp->next;
// 		i++;
// 	}
// 	return (0);
// }

// char	*ret_value_env(char *key, t_env *envp)
// {
// 	//printf("searching env\n");
	
// 	t_env	*tmp;

// 	tmp = envp;
// 	//go thru list until we find key
// 	while (ft_strncmp(tmp->key, key, ft_strlen(key)) != 0)
// 		tmp = tmp->next;
// 	//return value at node
// 	if (tmp)
// 		return ((char *)tmp->value);
// 	else
// 	{
// 		printf("no key in env found\n");
// 		return (NULL);
// 	}
// }

char	*extend_upper_dir(t_command *cmd) // ../
{
	char	*cwd;
	char	*sub;
	char	*joined;
	int		i;
	char	*cutpath;

	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (!cwd)
	{
	 	perror("error getting working dir: ");
		//free everything 
		return (NULL);
	}
	//printf("cwd: %s\n", cwd);
	//count to last / and cut off in cwd
	i = ft_strlen(cwd) - 1; //-1?
	//printf("i: %i\n", i);

	while (cwd[i] != '/')
		i--;
	
	//substr path 
	cutpath = ft_substr(cwd, 0, i);
	if (! cutpath)
		return NULL;
	//printf("cutpath: %s\n", cutpath);
	//substr arg - 2
	sub = ft_substr(cmd->args[0], 2, ft_strlen(cmd->args[0]) - 2);
	if (!sub)
		return (NULL);
	//printf("sub: %s\n", sub);
	//strjoin
	joined = ft_strjoin(cutpath, sub);
	//printf("joined: %s\n", joined);

	free(cwd);
	free(cutpath);
	free(sub);
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
	//printf("cwd: %s\n", cwd);

	//substr to get rid of .
	sub = ft_substr(cmd->args[0], 1, ft_strlen(cmd->args[0]) - 1);
	if (!sub)
	{
		return (NULL);
	}
	//printf("sub: %s\n", sub);
	//strjoin
	joined = ft_strjoin(cwd, sub);
	free(cwd);
	free(sub);
	return (joined);
}


void	check_path(t_command *cmd, t_env *envp) //TODO
{
	//printf("in check path \n");

	char	*fullpath; //whole path from envp
	char	*exec_path; //path that can be exec
	char	**paths;
	
	//0. check if ./ or ../
	if (cmd->args[0][0] == '.' && cmd->args[0][1] == '.' && cmd->args[0][2] == '/') // ../
	{		//printf("extending ../ \n");
		cmd->exec_path = extend_upper_dir(cmd);
		return;
	}
	if (cmd->args[0][0] == '.' && cmd->args[0][1] == '/') // ./
	{	//printf("extending ./ \n");
		cmd->exec_path = extend_current_dir(cmd);
		//printf("exec path: %s\n", cmd->exec_path);
		return;
	}
	else
	{
		//1. get whole path from env
		fullpath = ret_value_env("PATH", envp);
		//printf("fullpath: %s\n", fullpath);
		paths = ft_split(fullpath, ':');
		//2. get executable path if cmd not builtin
		exec_path = get_exec_path(cmd->args[0], paths); //returns malloced str if exists, NULL if not
		free_2d_char(&paths);
		//3. save exec path in cmd table
		cmd->exec_path = exec_path;
	}
}

int	count_env_size(t_env *envp)
{
	int		size;
	t_env	*tmp;
	
	tmp = envp;
	size = 0;
	if (envp)
	{
		size = 1;
		while (tmp->next)
		{
			tmp = tmp->next;
			size++;
		}
	}
	return (size);
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

char **convert_env_array(t_env *envp, t_pipeline *pipeline) //The envp array must be terminated by a NULL pointer.
{
	char	**array;
	int		lstsize;
	t_env	*tmp;
	int		i;
	char	*fullstr;

	//count size list
	lstsize = count_env_size(envp);
	//printf("size list is: %i\n", lstsize);

	//malloc space for pointer array + 1 for NULL
	array = (char **)malloc(sizeof(char *) * (lstsize + 1));
	if (!array)
		return (NULL);
	
	//go thru list and array
	tmp = envp;
	i = 0;
	while(tmp && i < lstsize)
	{
		//for each node in envp: 
			//create full string in form key=value
		fullstr = create_fullstr(tmp); //MALLOC
		if (!fullstr)
		{
			free_2d_char(&array);
			free_everything_pipeline_exit(envp, pipeline, 1);
		}
		//set full str to array[i]
		array[i] = fullstr;
		tmp = tmp->next;
		i++;
	}
	array[i] = NULL;

	// for (int y = 0; y <= lstsize; y++) { //debug only
    // printf("envp[%i]:%s\n", y, array[y]);
	// }

	return (array);
}
