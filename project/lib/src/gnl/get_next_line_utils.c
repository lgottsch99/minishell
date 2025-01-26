/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:56:18 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/02 15:32:12 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../full_libft.h"

int	getindexnl(char *leftover)
{
	int	index;

	index = 0;
	while (index < (int)ft_strlen(leftover))
	{
		if (leftover[index] == '\n')
			break ;
		index++;
	}
	return (index);
}
