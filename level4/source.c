#include <stdio.h>

int m = 0;  // 0x08049814

void	p(char *str) {  // 0x08048444
	printf(str);
}

void	n() {  // 0x08048457
	char buf[520];  // ebp-0x208 | esp+0x10

	fgets(buf, 512, stdin);  // 0x200
	p(buf);
	if (m == 0x1025544) {
	system("/bin/cat /home/user/level5/.pass");
	}
}

void	main() {  // 0x080484a7
	n();
}
