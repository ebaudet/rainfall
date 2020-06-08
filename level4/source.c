#include <stdio.h>

void	p(char *str) {
	printf(str);
}

void	n() {
	// char *fgets(char *s, int size, FILE *stream);
	char str[520];  // 0x208
	fgets(str, 512, stdin);  // 0x200
	p(str);
	void	*m = 0x8049810;  // bss segment finish at 0x08049814
	if (m == 0x1025544) {
		system("/bin/cat /home/user/level5/.pass");
	}
}

void	main() {
	n();
}
