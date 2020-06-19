#include <stdlib.h>

void	n() {  // 0x08048454
	system("/bin/cat /hom/user/level7/.pass");
}

void	m(char *str) {  // 0x08048468
	puts("Nope");
}

void	main(int ac, char **av) {  // 0x0804847c
	void *(*ptr_function)(char *);  // esp+0x18
	char *buf;  // esp+0x1c

	buf = malloc(64);
	ptr_function = malloc(4);
	ptr_function = &m;
	strcpy(buf, av[1]);
	(*ptr_function)(buf);
}
