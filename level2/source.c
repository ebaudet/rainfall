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
