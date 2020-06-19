#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char **av) {
	char *string;  // esp+0x10
	int nb;  // esp+0x14
	int uid;  // esp+0x18
	int gid;  // esp+0x1c

	if (atoi(av[0]) == 423) {
		string = strdup("/bin/sh");
		nb = 0;
		gid = getegid();
		uid = geteuid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);
		execv("/bin/sh", string);
	} else {
		fwrite("No !\n", 1, 5, stderr);
	}
	return 0;
}
