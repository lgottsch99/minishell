/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:43:57 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/17 15:54:37 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_start(void)
{
	printf("\n");
	printf(" .▄▄ ·  ▄ .▄▄▄▄ .▄▄▌  ▄▄▌      ▄▄▌ ▐ ▄▌▄▄▄ .\n");
	printf(" ▐█ ▀. ██▪▐█▀▄.▀·██•  ██•      ██· █▌▐█▀▄.▀·\n");
	printf(" ▄▀▀▀█▄██▀▐█▐▀▀▪▄██▪  ██▪      ██▪▐█▐▐▌▐▀▀▪▄\n");
	printf(" ▐█▄▪▐███▌▐▀▐█▄▄▌▐█▌▐▌▐█▌▐▌    ▐█▌██▐█▌▐█▄▄▌\n");
	printf("  ▀▀▀▀ ▀▀▀ · ▀▀▀ .▀▀▀ .▀▀▀      ▀▀▀▀ ▀▪ ▀▀▀  ??????\n");
	printf("\n");
	printf("\n");
	return ;
}

int	init_shell(t_env **environ, char *envp[])
{
	if (!envp || !envp[0])
	{
		printf("Failed to receive Environment from OS\n");
		printf("Exiting program\n");
		return (1);
	}
	print_start();
	*environ = set_env(envp);
	if (!environ)
	{
		printf("Failed to set up Environment\n");
		printf("Exiting program\n");
		return (1);
	}
	setup_signals();
	return (0);
}
