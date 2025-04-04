/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:26:09 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 13:55:00 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	wait_more(int status, t_pipeline *pipeline)
{
	if (WIFEXITED(status))
		*pipeline->exit_stat = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		*pipeline->exit_stat = 128 + WTERMSIG(status);
		write(STDOUT_FILENO, "\n", 1);
	}
}

static int	handle_wait_error(t_pipeline *pipeline, t_env *envp)
{
	if (errno == EINTR)
	{
		if (g_signal_status == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			return (1);
		}
		return (2);
	}
	free_everything_pipeline_exit(envp, pipeline, 1);
	return (0);
}

void	wait_children(t_pipeline *pipeline, t_env *envp)
{
	int		y;
	pid_t	waited_pid;
	int		status;
	int		action;

	y = 0;
	while (y < pipeline->nr_cmd)
	{
		waited_pid = waitpid(pipeline->pid[y], &status, 0);
		if (waited_pid == -1)
		{
			action = handle_wait_error(pipeline, envp);
			if (action == 1)
				break ;
			else if (action == 2)
				continue ;
		}
		if (y == pipeline->nr_cmd - 1)
			wait_more(status, pipeline);
		y++;
	}
}
