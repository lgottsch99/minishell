#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

int main (void)
{
	char *path = NULL;

	path = getcwd(path, PATH_MAX);
	printf("returned: %s\n", path);
	if (path)
		free(path);
}