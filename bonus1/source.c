#include <stdlib.h>
#include <string.h>

int		main(int ac, char **av) {  // 0x08048424
	char str[40];  // esp+0x14  --  size : 0x3c - 0x14
	int nb;  // esp+0x3c

	nb = atoi(av[1]);
	if (nb > 9) {
		return 1;
	}
	memcpy(str, av[2], 4 * nb);
	if (nb == 0x574f4c46) {
		execl("/bin/sh", "sh", 0);
	}
	return 0;
}
