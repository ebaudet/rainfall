#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char **av) {
	if (atoi(av[0]) == 423) {
		char *string = strdup("/bin/sh");
		int nb = 0;
		int gid = getegid();
		int uid = geteuid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);
		execv("/bin/sh", string);
	} else {
		fwrite("No !\n", 1, 5, stderr);
	}
	return 0;
}
