#include <stdio.h>

void main() {
	char string[64];
	gets(string);
}

void run() {
	fwrite("Good... Wait what?\n", 1, 19, stdout);
	system("/bin/sh");
}
