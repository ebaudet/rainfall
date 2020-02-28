#include <stdio.h>
#include <stdint.h>

int		v() {
	char str[0x208]; // 520
	fgets(str, 0x200, stdin);
	printf(str);
	void	*m = 0x804988c;
	if (m == 0x40) {
		fwrite("Wait what?!\n", 0x1, 0xc, stdout);
		system("/bin/sh");
	}
	return (m);
}

int		main(int ac, char **av, char **envp) {
	return(v());
}
