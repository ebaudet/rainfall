# Rainfall

> A 42 security project

## 1. Tools to access to the levels :

### SSH
> To connect to the server

`ssh level0@192.168.1.23 -p4242`

### SCP
> To download the file

`scp -P 4242 level0@192.168.1.23:level0 .`

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
