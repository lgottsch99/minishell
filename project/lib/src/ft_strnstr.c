/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 17:56:51 by lgottsch          #+#    #+#             */
/*   Updated: 2024/11/15 17:41:59 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../full_libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	y;

	if (ft_strlen(little) == 0)
		return ((char *)big);
	i = 0;
	while (i < len && big[i])
	{
		if (big[i] == little[0])
		{
			y = 0;
			while (big[i + y] == little[y] && i + y < len)
			{
				if (little[y + 1] == '\0')
					return ((char *)&big[i]);
				y++;
			}
		}
		i++;
	}
	return (NULL);
}
