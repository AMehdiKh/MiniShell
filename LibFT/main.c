#include <stdio.h>

int	main(void)
{
	char	path[100];

	path[100] = "~";
	printf("%d\n", ft_strchr(path, "~"));
	return (0);
}
