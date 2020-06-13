#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	p(char *str, char *sep) {
	char buf[4096];  // 0x1008 - 0x8
	puts(sep);
	read(stdin, buf, 4096);
	char *ptr = strchr(buf, '\n');
	*ptr = '\0';
	strncpy(str, buf, 20);  // 0x14
}

void	pp(char *buf) {
	char a[20];  // ebp-0x30
	p(a, " - ");

	char b[20];  // ebp-0x1c
	p(b, " - ");

	strcpy(buf, a);

	int i = 0;
	while (buf[i] != '\0') {
		i++;
	}
	buf[i] = ' ';

	strcat(buf, b);
}

int		main(int ac, char **av) {
	char buf[42];  // 0x40-0x16

	pp(buf);
	puts(buf);

	return (0);
}
