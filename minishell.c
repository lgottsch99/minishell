/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:49:05 by lgottsch          #+#    #+#             */
/*   Updated: 2025/01/13 18:48:06 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
A shell does three main things in its lifetime:
Initialize: 
    In this step, a typical shell would read and execute its configuration files. These change aspects of the shell’s behavior.
Interpret: 
    Next, the shell reads commands from stdin (which could be interactive, or a file) and executes them.
Terminate: 
    After its commands are executed, the shell executes any shutdown commands, frees up any memory, and terminates.

int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

INSIDE LOOP
REPL read evaluate print loop (loop only terminates when program is closed)

*/