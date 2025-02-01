/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:43:57 by lgottsch          #+#    #+#             */
/*   Updated: 2025/02/01 17:01:52 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void print_start(void)
{
	printf("  ______   __    __  ________  __        __       \n");
	printf(" /      \\ /  |  /  |/        |/  |      /  |      \n");
	printf("/$$$$$$  |$$ |  $$ |$$$$$$$$/ $$ |      $$ |      \n");
	printf("$$ \\__$$/ $$ |__$$ |$$ |__    $$ |      $$ |      \n");
	printf("$$      \\ $$    $$ |$$    |   $$ |      $$ |      \n");
	printf(" $$$$$$  |$$$$$$$$ |$$$$$/    $$ |      $$ |      \n");
	printf("/  \\__$$ |$$ |  $$ |$$ |_____ $$ |_____ $$ |_____ \n");
	printf("$$    $$/ $$ |  $$ |$$       |$$       |$$       |\n");
	printf(" $$$$$$/  $$/   $$/ $$$$$$$$/ $$$$$$$$/ $$$$$$$$/ \n");
	printf("                                                  \n");
	printf(" __      __ ________   ______   __    __          \n");
	printf("/  \\    /  /        | /      \\ /  |  /  |         \n");
	printf("$$  \\  /$$/$$$$$$$$/ /$$$$$$  |$$ |  $$ |         \n");
	printf(" $$  \\/$$/ $$ |__    $$ |__$$ |$$ |__$$ |         \n");
	printf("  $$  $$/  $$    |   $$    $$ |$$    $$ |         \n");
	printf("   $$$$/   $$$$$/    $$$$$$$$ |$$$$$$$$ |         \n");
	printf("    $$ |   $$ |_____ $$ |  $$ |$$ |  $$ |         \n");
	printf("    $$ |   $$       |$$ |  $$ |$$ |  $$ |         \n");
	printf("    $$/    $$$$$$$$/ $$/   $$/ $$/   $$/ ");
	printf("\n");
	printf("\n");
	printf("Welcome to our minishell!\n");
	printf("\n");
	printf("\n");
	return;
}

// //init char ** list with builtin names
// char	**builtin_list(void)
// { 
// 	//statically alloc list 
// 	char array[7][7]; //7 builtin cmds, longest one has 6 chars (+1 \0)

// //size_t	ft_strlcpy(char *dst, const char *src, size_t size)
// 	ft_strlcpy(array[0],"echo", 4);
// 	ft_strlcpy(array[1],"cd", 2);
// 	ft_strlcpy(array[2],"pwd", 3);
// 	ft_strlcpy(array[3],"export", 6);
// 	ft_strlcpy(array[4],"unset", 5);
// 	ft_strlcpy(array[5],"env", 3);
// 	ft_strlcpy(array[6],"exit", 4);

// 	// array[1] = "cd\0\0\0\0\0";
// 	// array[2] = "pwd\0\0\0\0";
// 	// array[3] = "export\0";
// 	// array[4] = "unset\0\0";
// 	// array[5] = "env\0\0\0\0";
// 	// array[6] = "exit\0\0\0";
// 	return (array);
// }
