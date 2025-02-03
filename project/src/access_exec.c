/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:24:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/03 18:24:00 by lgottsch         ###   ########.fr       */
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

int	check_access(t_command	*cmd_list, int nr_cmd, char *envp[])//ret 1 if access denied, 0 if ok
{
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

//check executability of cmds, (bash does not check if arguments to a cmd are valid!)
//create executable path and save in cmd table, else stay at NULL
void	check_path(t_command	*cmd, char *envp[])
{
	char	**path; //whole path from envp
	char	*exec_path; //path that can be exec

	//1. get whole path from env
	path = get_path(envp);
	//2. get executable path if cmd not builtin
	exec_path = get_exec_path(cmd->command, path); //returns malloced str if exists, NULL if not
	//3. save exec path in cmd table
	cmd->exec_path = exec_path;
	if(!exec_path)
		printf("nooo executable path found\n");
	else
		printf("executable path found\n");
}
