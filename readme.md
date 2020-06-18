# Rainfall

> A 42 security project

## The access to the differents levels

> level0 level0
> level1 1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
> level2 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
> level3 492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
> level4 b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
> level5 0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
> level6 d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
> level7 f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
> level8 5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
> level9 c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
> bonus0 f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
> bonus1 cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
> bonus2 579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
> bonus3 71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
> end    3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c

## 1. Tools to access to the levels :

### SSH
> To connect to the server

`ssh level0@192.168.1.23 -p4242`

### SCP
> To download the file

`scp -P 4242 level0@192.168.1.23:level0 .`

### How to pass args to programs

[Comment passer les arguments au programme ?](https://reverseengineering.stackexchange.com/questions/13928/managing-inputs-for-payload-injection/13929#13929)

### OBJDUMP

> Tool to display information from object files

- `objdump -t ./executable | grep bss` to display global vars
- `objdump -S ./executable` to display assembly source of executable
- `objdump -R ./executable` to display the GOT (Global Object Table)

## 2. GDB usage :

```
quit (q)				quitter gdb
run (r)					lancer l’exécution

break,watch,clear,delete (b,w,cl,d)	introduire un point d’arrêt, ou bien “surveiller” une variable

step,next,continue (s,n,c)		avancer d’un pas (en entrant ou pas dans les sous-fonctions), relancer
					jusqu’au prochain point d’arrêt
stepi (si)				steps by one machine instruction

print,backtrace,list (p,bt,l)    	afficher la valeur d’une variable, la pile d’exécution, afficher l’endroit où
								l’on se trouve dans le code

disassemble /m my_function		disas function my_function

info registers				print register values

x/20w $rsp				print 20 word a partir du pointeur de pile

commands				lists of commands to execute at each breakpoints

set $eax = 1			        set EAX register to 1

info functions                        liste toutes les fonctions due l'assembleur

info files        liste les différents segments dans le binaire.
```


* `EAX` Stores function return values
* `ECX` Counter for string and loop operation
* `EDX` I/O Pointer
* `EIP` Instruction pointer (Pointer to the next instruction to be executed)
* `EBX` Base pointer to the data section (in DS segment)
* `EDI` Source pointer for string operations
* `ESI` Pointer to data in the sement pointed by the DS register; Destination pointer for string operations.
* `ESP` Stack pointer (in the SS Segment)
* `EBP` Stack frame base pointer (local variables in the function) (in the SS Segment)

## 3. Peda usage

```bash
git clone https://github.com/longld/peda
echo "source /tmp/peda/peda.py" >> peda/gdbinit
sshpass -p level0  scp -r -P 4242 peda level0@10.11.200.132:/tmp/peda
```

Puis sur la machine distante :

```bash
gdb -x /tmp/peda/gdbinit ./file_to_gdb
```

usage : https://github.com/longld/peda



### Pattern :

`pattern create 100` \
`pattern search`

```bash
gdb-peda$ pattern create 100
'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL'
gdb-peda$ r
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAJAAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x804a008 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAJAAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
EBX: 0xb7fd0ff4 --> 0x1a4d7c
ECX: 0x0
EDX: 0xbffff69c ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAJAAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
ESI: 0x0
EDI: 0x0
EBP: 0x41344141 ('AA4A')
ESP: 0xbffff6f0 ("fAA5AAKAAgAA6AAL")
EIP: 0x41414a41 ('AJAA')
EFLAGS: 0x210282 (carry parity adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41414a41
[------------------------------------stack-------------------------------------]
0000| 0xbffff6f0 ("fAA5AAKAAgAA6AAL")
0004| 0xbffff6f4 ("AAKAAgAA6AAL")
0008| 0xbffff6f8 ("AgAA6AAL")
0012| 0xbffff6fc ("6AAL")
0016| 0xbffff700 --> 0x0
0020| 0xbffff704 --> 0xbffff794 --> 0xbffff8ba ("/home/user/level2/level2")
0024| 0xbffff708 --> 0xbffff79c --> 0xbffff8d3 ("SHELL=/bin/bash")
0028| 0xbffff70c --> 0xb7fdc858 --> 0xb7e2c000 --> 0x464c457f
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41414a41 in ?? ()
gdb-peda$ pattern search
Registers contain pattern buffer:
EIP+0 found at offset: 80
EBP+0 found at offset: 76
Registers point to pattern buffer:
[EAX] --> offset 0 - size ~100
[EDX] --> offset 0 - size ~100
[ESP] --> offset 84 - size ~16
Pattern buffer found at:
0x0804a04b : offset   67 - size   33 ([heap])
0xb7fd9043 : offset   67 - size   33 (mapped)
0xb7fda000 : offset    0 - size  100 (mapped)
0xbffff6df : offset   67 - size   33 ($sp + -0x11 [-5 dwords])
References to pattern buffer found at:
0xb7fd1acc : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1ad0 : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1ad4 : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1ad8 : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
0xb7fd1adc : 0xb7fda000 (/lib/i386-linux-gnu/libc-2.15.so)
```

## 4. Found the iso :

```
$ for i in {0..9999}; do echo -n $i' ' ; curl -Is "https://projects.intra.42.fr/uploads/document/document/$i/RainFall.iso" -o /dev/null -w %{http_code} | grep -v 404; echo ''; done;
...
328
329
330
331 200

332
333
334
335
336
...
```
=> https://projects.intra.42.fr/uploads/document/document/331/RainFall.iso
