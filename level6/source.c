#include <stdlib.h>

void	n() {
	system("/bin/cat /hom/user/level7/.pass");
}

void	m(char *str) {
	puts("Nope");
}

void	main(int ac, char **av) {
	char *str = malloc(64);  // 0x40
	void * (*ptr_function)(char *);
	ptr_function = malloc(4);
	ptr_function = &m;
	strcpy(str, av[1]);
	(*ptr_function)(str);
}
