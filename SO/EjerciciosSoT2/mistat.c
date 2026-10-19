#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/sysmacros.h>

int main(int argc, char *argv[]) {
	struct stat datos;
	char *tipo;

	if (argc != 2) {
	fprintf(stderr, "Uso: %s ruta_del_fichero\n", argv[0]);
	return 1;
	}

	if (lstat(argv[1], &datos) == -1) {
	fprintf(stderr, "Error en stat: %s\n", strerror(errno));
	return 1;
	}

	printf("inodo: %ld\n", (long) datos.st_ino);

	printf("Número de dispositivo: [%u,%u]\n",
		major(datos.st_dev), minor(datos.st_dev));

	if (S_ISDIR(datos.st_mode)) {
	tipo = "directorio";
	} else if (S_ISLNK(datos.st_mode)) {
	tipo = "enlace simbólico";
	} else if (S_ISREG(datos.st_mode)) {
	tipo = "fichero regular";
	} else {
	tipo = "otro";
	}

	printf("Tipo: %s\n", tipo);

	printf("Último acceso: %s",
		ctime(&datos.st_atime));

	return 0;
}
