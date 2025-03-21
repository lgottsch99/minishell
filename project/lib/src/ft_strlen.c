/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:11:53 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/15 17:41:28 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../full_libft.h"

size_t	ft_strlen(const char *s)
{
	// int	i;

	// i = 0;
	// while (s[i])
	// 	i++;
	// return (i);
	if (!s)  // Handle NULL pointer
        return 0;

    size_t len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}
