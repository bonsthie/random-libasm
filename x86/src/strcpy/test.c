#include <stdio.h>
#include <stdlib.h>

char	*__strcpy_base(char *dest, char *src);

int main() {
	char *str = malloc(100);
	char src[] = "crampte\n";

	printf("%s", __strcpy_base(str, src));
}
