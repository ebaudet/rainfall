#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

char c[80];  // 0x08049960 / size : 0x50

void	m() {  // 0x080484f4
	printf("%s - %d", c, time(0));
}

int		main(int ac, char **av) {  // 0x08048521
	char **b;  // esp+0x18
	char **a;  // esp+0x1c

	a = malloc(2 * sizeof(char *));
	a[0] = 1;
	a[1] = malloc(8);

	b = malloc(2 * sizeof(char *));
	b[0] = 2;
	b[1] = malloc(8);

	strcpy(a[1], av[1]);
	strcpy(b[1], av[2]);

	fgets(c, 68, fopen("/home/user/level8/.pass", "r"));  // 0x44

	// 0x8048703 on .rodata segment
	puts("~~");

	return (0);
}
