#include <stdio.h>
#include <sys/utsname.h>

int main() {
	struct utsname info;

	if (uname(&info) == -1) {
	perror("uname");
	return 1;
	}

	printf("Nombre SO: %s\n", info.sysname);
	printf("Version SO: %s\n", info.release);
	printf("Nombre Host: %s\n", info.nodename);
	printf("Arquitectura: %s\n", info.machine);

	return 0;
}
