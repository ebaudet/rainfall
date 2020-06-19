#include <stdio.h>
#include <stdint.h>

int	m = 0;  // 0x804988c

int		v() {  // 0x080484a4
	char str[520];  // ebp-0x208 | esp+0x10 / size : 0x208

	fgets(str, 512, stdin);  // 0x200
	printf(str);
	if (m == 64) {  // 0x40
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
	return m;
}

int		main(int ac, char **av, char **envp) {  // 0x0804851a
	return v();
}
