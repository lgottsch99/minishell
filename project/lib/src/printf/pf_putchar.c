/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 18:02:21 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/02 15:27:42 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../full_libft.h"

int	pf_putchar(char c, int fd, int *count)
{
	int	error;

	error = write(fd, &c, 1);
	*count += 1;
	if (error != 1)
		return (-1);
	else
		return (0);
}
