#include <stdio.h>

// 0x080484a4
void	o() {
	system("/bin/sh");
	_exit(1);
}

// 0x080484c2
void	n() {
	// char *fgets(char *s, int size, FILE *stream);
	char	str[520];  // 0x208
	fgets(str, 512, stdin);  // 0x200
	printf(str);
	exit(1);
}

// 0x08048504
void	main() {
	n();
}
