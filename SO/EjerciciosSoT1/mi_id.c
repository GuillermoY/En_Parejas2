#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	uid_t uid;
	
	if (argc == 1) {
		uid = getuid();
	} else if (argc == 2) {
		uid = atoi(argv[1]);
	} else {
		printf("Uso: %s [UID]\n", argv[0]);
		return 1;
	}
	
	struct passwd *usuario = getpwuid(uid);
	
	if (usuario == NULL) {
		printf("Usuario no existe\n");
		return 1;
	}

	printf("Nombre de usuario: %s\n", usuario->pw_name);
	printf("ID de usuario, grupo: %d,%d\n", usuario->pw_uid, usuario->pw_gid);
	printf("Directorio home: %s\n", usuario->pw_dir);

	return 0;
}
