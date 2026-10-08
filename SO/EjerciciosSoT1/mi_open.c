#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Uso: %s <fichero>\n", argv[0]);
		return 1;
	}

	int fd = open(argv[1], O_RDONLY);
	
	if (fd == -1) {
	printf("Código de error: %d\n", errno);
	printf("Mensaje: %s\n", strerror(errno));
	return 1;
	}

	return 0;
}
