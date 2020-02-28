#include <stdio.h>
#include <stdint.h>

int		p() {
	char	buf[64];
	int32_t	local_arg;

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

int		main(int ac, char **av, char **envp) {
	return(p());
}
