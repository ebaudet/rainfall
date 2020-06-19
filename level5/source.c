#include <stdio.h>

int m = 0;  // 0x08049854

void	o() {  // 0x080484a4
	system("/bin/sh");
	_exit(1);
}

void	n() {  // 0x080484c2
	char	buf[520];  // ebp-0x208 | esp+0x10

	fgets(buf, 512, stdin);  // 0x200
	printf(buf);
	exit(1);
}

void	main() {  // 0x08048504
	n();
}
