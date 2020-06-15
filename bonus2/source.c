#include <string.h>
#include <stdlib.h>

int language = 0;

int		greetuser(char *name) {
	char str[72];  // ebp-0x48 == esp+0x10
	switch (language) {
		case 0:
			strcpy(str, "Hello ");
			break;
		case 1:
			strcpy(str, "Hyvää päivää ");
			break;
		case 2:
			strcpy(str, "Goedemiddag! ");
			break;
		default:
			break;
	}
	strcat(str, name);
	return (puts(str));
}

int		main(int ac, char **av) {
	char str[76];  // esp+0x50
	char str2[80];  // esp
	char *lang;  // esp+0x9c

	if (ac != 3)
		return 1;

	memset(str, '\0', 19);  // 0x13

	stpncpy(str, av[1], 40);  // 0x28
	strncpy(str + 40, av[2], 32);  // x020
	lang = getenv("LANG");
	if (*lang) {
		if (!memcmp(lang, "fi", 2)) {
			language = 1;
		} else {
			if (!memcmp(lang, "nl",2)) {
				language = 2;
			}
		}
	}
	for (int i = 0; i < 19; i++) {
		str2[i] = str[i];
	}

	return greetuser(str2);
}
