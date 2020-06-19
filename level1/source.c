#include <stdio.h>

void main() {  // 0x08048480
	char string[64];  // esp+0x10 - size = 0x50-0x10
	gets(string);
}

void run() {  // 0x08048444
	fwrite("Good... Wait what?\n", 1, 19, stdout);
	system("/bin/sh");
}
