#include <stdio.h>
#include <string.h>

char	*auth = NULL;  // 0x08049aac
char	*service = NULL;  // 0x08049ab0

int	main() {  // 0x08048564
	char	buf[128];  // esp+0x20 / size = 0xa0 - 0x20

	while (1) {
		printf("%p, %p \n", auth, service);
		if (fgets(buf, 128, stdin) == 0)
			break;
		if (memcmp(buf, "auth ", 5) == 0) {
			auth = malloc(4);
			*auth = '\0';
			if (strlen(&buf[5]) <= 30) {
				strcpy(auth, &buf[5]);
			}
		}
		if (memcmp(buf, "reset", 5) == 0) {
			free(auth);
		}
		if (memcmp(buf, "service", 7) == 0) {
			service = strdup(&buf[7]);
		}
		if (memcmp(buf, "login") == 0) {
			if (auth[32] == '\0') {
				system("/bin/sh");
			} else {
				fwrite("Password:\n", 1, 10, stdout);
			}
		}
	}
	return (0);
}
