#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

char c[80];  // 0x00000050

void	m() {
	// time_t time(time_t *tloc);
	time_t duration = time(0);

	printf("%s - %d", c, duration);
}

int		main(int ac, char **av) {
	// void *malloc(size_t size);
	char **a;  // esp+0x1c
	a = malloc(2 * sizeof(char *));
	a[0] = 1;
	a[1] = malloc(8);

	char **b;  // esp+0x18
	b = malloc(2 * sizeof(char *));
	b[0] = 2;
	b[1] = malloc(8);

	// char *strcpy(char *dest, const char *src);
	strcpy(a[1], av[1]);
	strcpy(b[1], av[2]);

	// FILE *fopen(const char *pathname, const char *mode);
	FILE *file;
	file = fopen("/home/user/level8/.pass", "r");

	// char *fgets(char *s, int size, FILE *stream);
	fgets(c, 68, file);  // 0x44

	// 0x8048703 on .rodata segment
	puts("~~");

	return (0);
}
