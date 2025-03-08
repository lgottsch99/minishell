//HEADER

#include "../full_libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;
	int		x;

	i = 0;
	while (s1[i] && s2[i])
	{
		if ((s1[i] != s2[i]) || !s1[i] || !s2[i])
		{
			x = ((unsigned char)s1[i] - (unsigned char)s2[i]);
			return (x);
		}
		i++;
	}
	return (0);
}