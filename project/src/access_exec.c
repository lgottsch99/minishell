/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:24:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/14 14:12:43 by lgottsch         ###   ########.fr       */
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

/*
typedef struct s_command {
    char	*command;       // The command name (e.g., "echo", "grep")
    char	**args;         // Array of arguments (NULL-terminated)
    char	*input_file;    // File for input redirection (NULL if none)
    char	*output_file;   // File for output redirection (NULL if none)
    int		append_mode;     // 1 if output should be appended, 0 otherwise
	//..more if needed:
	char 	*exec_path; // NULL for parsing, execution: saves executable path in here
	int		is_builtin;	//0 for parsing, exec: 0 if not, 1 if yes

    struct s_command *next; // Pointer to the next command in a pipeline
} t_command;
*/

int	check_files(t_command *cmd) //ret 1 if denied, 0 if ok
{
	int	infile;

	printf("in check files\n");
	
	infile = 1;


	//read access infile
	if (access(cmd->input_file, F_OK) == 0) //file 1 need to exist
	{
		if (access(cmd->input_file, R_OK) == 0) //read access ok
			infile = 0;
		else 
			printf("read access to %s denied\n", cmd->input_file);
	}
	else 
		printf("%s does not exist\n", cmd->input_file);
	
	//write access outfile  ???needed???
	
	return (infile);
}

int	check_access(t_command	*cmd_list, int nr_cmd, t_env *envp)//ret 1 if access denied, 0 if ok
{
	printf("in check access \n");

	int			i;
	int			builtin;
	t_command	*tmp; //to trav list

	tmp = cmd_list;
	i = 0;
	while (i < nr_cmd)	//for each cmd in list
	{
		builtin = 0;
		//CHECK IF CMD IS BUILTIN
		builtin = check_builtin(tmp->command);
		//IF NOT BUILTIN:
		if (builtin == 0)
			check_path(tmp, envp);
		else if (builtin == 1)
		{
			printf("cmd is builtin: %s\n", tmp->command);
			tmp->is_builtin = 1;
		}
		if (!tmp->exec_path && builtin == 0) //cmd not found
		{
			printf("minishell: error: cant find command\n");
			return (1);
		}
		//check access to files
		if (tmp->input_file)
		{
			if (check_files(tmp) == 1)
				return (1);
		}
		tmp = tmp->next;
		i++;
	}
	return (0);
}

char	*ret_value_env(char *key, t_env *envp)
{
	t_env	*tmp;

	tmp = envp;
	//go thru list until we find key
	while (ft_strncmp(tmp->key, key, ft_strlen(key)) != 0)
	{
		tmp = tmp->next;
	}
	//return value at node
	if (tmp)
		return ((char *)tmp->value);
	else
	{
		printf("no key in env found\n");
		return (NULL);
	}
}


void	check_path(t_command	*cmd, t_env *envp) //TODO
{
	printf("in check path \n");

	char	*fullpath; //whole path from envp
	char	*exec_path; //path that can be exec
	char	**paths;

	//1. get whole path from env
	fullpath = ret_value_env("PATH", envp);
	printf("fullpath: %s\n", fullpath);
	paths = ft_split(fullpath, ':');
	//2. get executable path if cmd not builtin
	exec_path = get_exec_path(cmd->command, paths); //returns malloced str if exists, NULL if not
	//3. save exec path in cmd table
	cmd->exec_path = exec_path;
	if(!exec_path)
		printf("nooo executable path found\n");
	else
		printf("executable path found\n");
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

char	*create_fullstr(t_env *node)
{
	int		fullsize;
	char	*fullstr;
	int		i;

	//get size of full
	fullsize = ft_strlen(node->key) + ft_strlen(node->value) + 2; //2 bc one = and one \0
	//malloc
	fullstr = (char *)malloc(sizeof(char) * fullsize);
	//if (!fullstr)
		//return (NULL);
	//copy key
	i = 0;
	while (i < (int)ft_strlen(node->key))
	{
		fullstr[i] = node->key[i];
		i++;
	}
	//copy =
	fullstr[i] = '=';
	i++;
	//copy value
	while (i < fullsize - 1)
	{
		fullstr[i] = node->value[i];
		i++;
	}
	fullstr[i] = '\0';
	return (fullstr);
}

char **convert_env_array(t_env *envp) //The envp array must be terminated by a NULL pointer.
{
	char	**array;
	int		lstsize;
	t_env	*tmp;
	int		i;
	char	*fullstr;

	//count size list
	lstsize = count_env_size(envp);
	printf("size list is: %i\n", lstsize);
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
		fullstr = create_fullstr(tmp);
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
