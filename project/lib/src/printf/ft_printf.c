/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:13:13 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/02 15:27:26 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../full_libft.h"

/*
behaviour std printf:
	only writes to stdout (=terminal) fd 1
	returns -1 on error!
use:
	malloc, free, write,
	va_start, va_arg, va_copy, va_end (va_list is datatype, so ok to use)
handle:
	% cspdiuxX%
*/
static int	check_case(const char *s, int i, va_list ap, int *count);

int	ft_printf(const char *s, ...)
{
	int		i;
	int		count;
	int		error;
	va_list	ap;

	error = 0;
	va_start(ap, s);
	count = 0;
	i = 0;
	while (i < (int)ft_strlen(s))
	{
		if (s[i] == '%')
		{
			error = error + check_case(s, i, ap, &count);
			i = i + 2;
		}
		else
			error = error + pf_putchar(s[i++], 1, &count);
	}
	va_end(ap);
	if (error < 0)
		return (-1);
	else
		return (count);
}

static int	check_case(const char *s, int i, va_list ap, int *count)
{
	int	error;

	error = 0;
	if (s[i + 1] == 'c')
		error = pf_putchar((char)va_arg(ap, int), 1, count);
	if (s[i + 1] == 's')
		error = pf_putstr((char *)va_arg(ap, char *), count);
	if (s[i + 1] == 'p')
		error = ft_memaddress(va_arg(ap, void *), count);
	if (s[i + 1] == 'd' || s[i + 1] == 'i')
		error = pf_putnbr(va_arg(ap, int), 1, count);
	if (s[i + 1] == 'u')
		error = ft_putuint(va_arg(ap, unsigned int), count);
	if (s[i + 1] == 'x' || s[i + 1] == 'X')
		error = ft_putnbr_base(va_arg(ap, unsigned int), s[i + 1], count);
	if (s[i + 1] == '%')
		error = pf_putchar('%', 1, count);
	return (error);
}
