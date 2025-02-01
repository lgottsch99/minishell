/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:24:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/01 17:13:37 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **get_path(char *envp[]) //find path in envp and extract full path
{
	int i;
	char *fullpath;
    char **paths;
	
	i = 0;
	while (envp[i])
	{
		if ((ft_strnstr(envp[i], "PATH=", ft_strlen(envp[i])))!= NULL)
		{
			if (envp[i][0]== 'P' && envp[i][1]== 'A' && envp[i][2] == 'T' && envp[i][3] == 'H')
			{
				fullpath = ft_substr(envp[i], 5, (ft_strlen(envp[i]))); // !!!!malloc
                paths = ft_split(fullpath, ':');
                free(fullpath);
				return (paths);
			}
		}
		i++;
	}
	return (NULL);
}

char *get_exec_path(char *cmd, char **path)
{
    char *tmp;
    char *commandpath; //zb /usr/bin/ls

    while (*path)
    {
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

int	check_builtin(char *cmd) //ret 1 if yes, 0 if no
{	//int	ft_strncmp(const char *s1, const char *s2, size_t n) //ret 0 if the same
	
	if ((ft_strncmp(cmd, "echo", 4) == 0) && ft_strlen(cmd) == 4) //dont want to treat echooo as same as echo!
		return (1);
	if ((ft_strncmp(cmd, "cd", 2) == 0) && ft_strlen(cmd) == 2)
		return (1);
	if ((ft_strncmp(cmd, "pwd", 3) == 0) && ft_strlen(cmd) == 3)
		return (1);
	if ((ft_strncmp(cmd, "export", 6) == 0) && ft_strlen(cmd) == 6)
		return (1);
	if ((ft_strncmp(cmd, "unset", 5) == 0) && ft_strlen(cmd) == 5)
		return (1);
	if ((ft_strncmp(cmd, "env", 3) == 0) && ft_strlen(cmd) == 3)
		return (1);
	if ((ft_strncmp(cmd, "exit", 4) == 0) && ft_strlen(cmd) == 4)
		return (1);
	return (0);
}
