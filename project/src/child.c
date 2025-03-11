/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 14:05:26 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/11 21:02:14 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_process(t_pipeline *pipeline, t_env *envp, int i, t_command *tmp)
{
	printf("in child\n");

//close ALLLLLL not needed pipe ends
	close_not_needed_pipe(i, pipeline);

//redirections
	child_redirection(tmp, i, pipeline, envp);

//exec and leave process 
	//if NOT BUILTIN
	if (tmp->is_builtin == 0)
	{
		pipeline_exec(pipeline, tmp, envp);
	}
	else	//If BUILTIN
	{
		pipeline_builtin(pipeline, tmp, i, envp);
	}

}

void	pipeline_builtin(t_pipeline *pipeline, t_command *tmp, int i, t_env *envp)
{
	printf("its a builtin\n");

	if (ft_strncmp(tmp->args[0], "exit", ft_strlen(tmp->args[0])) == 0)//close fd if builtin exit???
	{
		printf("clsoing fds before exit\n");
		close_child_fds(pipeline, i);
	}
	run_builtin(tmp, envp, pipeline, pipeline->exit_stat); //return 0 or error nr?
	//*pipeline->exit_stat = run_builtin(tmp, envp, pipeline		
	//close open fds
	close_child_fds(pipeline, i);

	//free
	free_everything_pipeline_exit(envp, pipeline, *pipeline->exit_stat);
}


void	pipeline_exec(t_pipeline *pipeline, t_command *tmp, t_env *envp)
{
	//printf("going to execve\n");
	//convert t_list envp into char ** for execve
	pipeline->env_array = convert_env_array(envp, pipeline); //MALLOC
	if (!pipeline->env_array)
	{
		perror("malloc env_array: ");
		free_everything_pipeline_exit(envp, pipeline, 1);
		//free more?
	}
	
	printf("converted env to array\n");
	//print_commands(tmp);//remove
	if(execve(tmp->exec_path, tmp->args, pipeline->env_array) == -1) //execve closing open fds? - yes
	{
		perror("execve: \n");
		free_2d_char(pipeline->env_array);
		free_everything_pipeline_exit(envp, pipeline, 1); //1 as exit tstat ok??
		///free more?
	}
}
