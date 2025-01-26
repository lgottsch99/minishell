/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 13:47:04 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/15 17:38:36 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../full_libft.h"

/*
Allocates (with malloc(3)) and returns a string
representing the integer received as an argument.
Negative numbers must be handled.
use: malloc
return: The string representing the integer.
NULL if the allocation fails.
*/

static size_t	ft_length(int n)
{
	size_t	x;
	long	num;

	num = n;
	x = 0;
	if (num < 0)
	{
		x++;
		num = num * (-1);
	}
	while (num)
	{
		num = num / 10;
		x++;
	}
	return (x);
}

static char	*ft_switch(char *itoa, size_t x)
{
	size_t	i;
	char	tmp;

	i = 0;
	while (i < (x / 2))
	{
		tmp = itoa[i];
		itoa[i] = itoa[x - i - 1];
		itoa[x - i - 1] = tmp;
		i++;
	}
	return (itoa);
}

static char	*ft_doit(char *itoa, long num, int n)
{
	size_t	i;

	i = 0;
	if (num < 0)
	{
		num = num * (-1);
	}
	while (num)
	{
		itoa[i] = (num % 10) + 48;
		num = num / 10;
		i++;
	}
	if (n < 0)
	{
		itoa[i] = '-';
		i++;
	}
	itoa[i] = '\0';
	return (itoa);
}

char	*ft_itoa(int n)
{
	size_t	x;
	char	*itoa;
	long	num;

	if (n == 0)
		return (ft_strdup("0"));
	x = ft_length(n);
	itoa = (char *)malloc(sizeof(char) * (x + 1));
	if (!itoa)
		return (NULL);
	num = n;
	itoa = ft_doit(itoa, num, n);
	itoa = ft_switch(itoa, x);
	return (itoa);
}
