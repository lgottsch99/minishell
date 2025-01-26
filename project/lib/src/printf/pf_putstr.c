/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:33:29 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/02 15:27:47 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../full_libft.h"

int	pf_putstr(char *s, int *count)
{
	int	i;
	int	error;

	error = 0;
	i = 0;
	if (s == NULL)
	{
		error = pf_putstr("(null)", count);
		return (0);
	}
	while (s[i])
	{
		error = pf_putchar(s[i], 1, count);
		i++;
	}
	return (error);
}
