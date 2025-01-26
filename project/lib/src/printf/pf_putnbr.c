/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:54:29 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/02 15:27:44 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../full_libft.h"

int	pf_putnbr(int n, int fd, int *count)
{
	char	array[10];
	int		i;
	long	num;
	int		error;

	num = n;
	if (num == 0)
	{
		error = pf_putchar('0', fd, count);
		return (error);
	}
	if (num < 0)
	{
		error = pf_putchar('-', fd, count);
		num = num * (-1);
	}
	i = 0;
	while (num)
	{
		array[i++] = (num % 10) + 48;
		num = num / 10;
	}
	while (i-- > 0)
		error = pf_putchar(array[i], fd, count);
	return (error);
}
