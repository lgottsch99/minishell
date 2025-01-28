/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:55:48 by lgottsch          #+#    #+#             */
/*   Updated: 2025/01/28 18:30:58 by Watanudon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

//---------- external libraries ------------------
#include <stdio.h>  	//printf, readline
#include <unistd.h> 	//isatty, exit
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>		//malloc, free

//---------- Macros ------------------



//---------- Structs ------------------

typedef struct s_command {
    char	*command;       // The command name (e.g., "echo", "grep")
    char	**args;         // Array of arguments (NULL-terminated)
    char	*input_file;    // File for input redirection (NULL if none)
    char	*output_file;   // File for output redirection (NULL if none)
    int		append_mode;     // 1 if output should be appended, 0 otherwise
	//..more if needed

    struct s_command *next; // Pointer to the next command in a pipeline
} t_command;


//---------- functions ------------------

//00_init_start
void	print_start(void);

//builtins
void	print_env(char *envp[]);

//execution
void	execute(char *envp[]);



#endif