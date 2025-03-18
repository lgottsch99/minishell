/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 14:05:26 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/18 15:39:14 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_process(t_pipeline *pipeline, t_env *envp, int i, t_command *tmp)
{
	close_not_needed_pipe(i, pipeline);
	child_redirection(tmp, i, pipeline, envp);
	if (tmp->is_builtin == 0)
		pipeline_exec(pipeline, tmp, envp);
	else
		pipeline_builtin(pipeline, tmp, i, envp);
}

void	pipeline_builtin(t_pipeline *pipeline, t_command *tmp,
	int i, t_env *envp)
{
	if (ft_strncmp(tmp->args[0], "exit", ft_strlen(tmp->args[0])) == 0)
		close_child_fds(pipeline, i);
	run_builtin(tmp, envp, pipeline, pipeline->exit_stat);
	close_child_fds(pipeline, i);
	free_everything_pipeline_exit(envp, pipeline, *pipeline->exit_stat);
}

void	pipeline_exec(t_pipeline *pipeline, t_command *tmp, t_env *envp)
{
	pipeline->env_array = convert_env_array(envp, pipeline);
	if (!pipeline->env_array)
	{
		perror("malloc env_array: ");
		free_everything_pipeline_exit(envp, pipeline, 1);
	}
	if (execve(tmp->exec_path, tmp->args, pipeline->env_array) == -1)
	{
		perror("execve: \n");
		free_2d_char(pipeline->env_array);
		free_everything_pipeline_exit(envp, pipeline, 1);
	}
}
