/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:49:05 by lgottsch          #+#    #+#             */
/*   Updated: 2025/01/20 18:17:26 by Watanudon        ###   ########.fr       */
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
REPL read, evaluate, print, loop (loop only terminates when program is closed)

1. read the line (into buffer? ggf needed to make bigger dynamically?)
2. evaluate/parse the line (->tokenization: check how ft strtok works) (pipes? ""? redirections? ...)
3. execute


??? 
minishell = interactive bc we ask for propmt? or can all shells be run in both?
what is a good hierarchy to parse and structure program? like 1. redirections, then pipes then etc?
what are signals??? like user interrupt?
idea:
parse and count pipes ->set loop for pipes

First meeting: 
how to work together? regular meetings? communication? github?
how to structure the project?
*/