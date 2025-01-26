/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:56:45 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/15 17:39:49 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../full_libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*tmp;
	unsigned char	*helpd;

	if (dest == NULL && src == NULL)
		return (dest);
	tmp = (unsigned char *)src;
	helpd = (unsigned char *)dest;
	while (n)
	{
		*helpd = *tmp;
		tmp++;
		helpd++;
		n--;
	}
	return (dest);
}
