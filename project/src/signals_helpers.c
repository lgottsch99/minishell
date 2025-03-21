/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:08:42 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/21 18:32:55 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void handle_sigint(int sig)
{
    (void)sig;
    g_signal_status = SIGINT;

    //if (getpid() == getpgrp()) //  !!!both not allowed?
	if (isatty(STDIN_FILENO))
	{
        //write(STDOUT_FILENO, "\nHANDLED IN PARENT\n", 19);
        //rl_replace_line("", 0);
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_redisplay();
		//fflush(stdout); //  !!! not allowed?
    }
    else
    {
        //write(STDOUT_FILENO, "\nHANDLED IN CHILD\n", 18);
        exit(130);
    }
}



void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}