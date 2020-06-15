# Bonus2:

pass : `579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245`

## Sources :
```c=
#include <string.h>
#include <stdlib.h>

int language = 0;

int		greetuser(char *name) {
	char *str;
	switch (language) {
		case 0:
			str = "Hello ";
			break;
		case 1:
			str = "Hyvää päivää ";
			break;
		case 2:
			str = "Goedemiddag! ";
			break;
		default:
			break;
	}
	strcat(str, name);
	return (puts(str));
}

int		main(int ac, char **av) {
	char s1[76];  // esp+0x50
	char *s2;  // esp+0x9c

	if (ac != 3)
		return 1;

	memset(s1, '\0', 19);  // 0x13

	stpncpy(s1, av[1], 40);  // 0x28
	strncpy(s1 + 40, av[2], 32);  // x020
	s2 = getenv("LANG");
	if (*s2) {
		if (!memcmp(s2, "fi", 2)) {
			language = 1;
		} else {
			if (!memcmp(s2, "nl",2)) {
				language = 2;
			}
		}
	}
	return greetuser(s1);
}
```

## Recherche :


## Exploit :

```shell=

```
