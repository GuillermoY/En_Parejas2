#include <stdio.h>

char * init_string() {
	static char s[] = "Esto es una cadena";
	return s;
}

int main(){
	char * msg = init_string();
	printf("Cadena: %s\n", msg);
	return 0;
}
