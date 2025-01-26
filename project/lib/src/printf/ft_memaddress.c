/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memaddress.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:59:35 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/02 15:25:51 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../full_libft.h"
//get mem address of void ptr and print it in lower hex
int	ft_memaddress(void *ptr, int *count)
{
	int	error;
	int	one;
	int	two;

	if (ptr == NULL)
	{
		error = pf_putstr("(nil)", count);
		return (error);
	}
	else
	{
		error = pf_putchar('0', 1, count);
		one = pf_putchar('x', 1, count);
		two = ft_putnbr_base((unsigned long long)ptr, 'x', count);
		if (error == -1 || one == -1 || two == -1)
			return (-1);
		else
			return (0);
	}
}
