#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int		main(int ac, char **av) {  // 0x080484f4
	FILE	*file;  // exp+0x9c
	char	str[65];  // esp+0x18
	int		nb;  // esp+0x59

	file = fopen("/home/user/end/.pass", "r");
	bzero(str, 33);
	if (!file)
		return 255;
	if (ac != 2)
		return 255;

	fread(str, 1, 66, file);  // 0x42
	nb = 0;
	str[atoi(av[1])] = 0;
	fread(str+66, 1, 65, str);  // 0x41
	fclose(file);

	if (strcmp(str, av[1]) == 0) {
		execl("/bin/sh", "sh", 0);
	} else {
		puts(str + 66);
	}
	return 0;
}
