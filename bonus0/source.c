#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	p(char *str, char *sep) {  // 0x080484b4
	char buf[4096];  // ebp-1008 | esp+0x10 / size : 0x1008 - 0x8
	puts(sep);
	read(stdin, buf, 4096);
	char *ptr = strchr(buf, '\n');
	*ptr = '\0';
	strncpy(str, buf, 20);  // 0x14
}

void	pp(char *buf) {  // 0x0804851e
	char a[20];  // ebp-0x30 | esp+0x20
	char b[20];  // ebp-0x1c | esp+0x34

	p(a, " - ");
	p(b, " - ");
	strcpy(buf, a);
	buf[strlen(buf)] = ' ';
	strcat(buf, b);
}

int		main(int ac, char **av) {  // 0x080485a4
	char buf[42];  // esp+0x16 / size : 0x40-0x16

	pp(buf);
	puts(buf);
	return (0);
}
