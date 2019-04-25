#include "estructuras.h"

t_esi* new_esi(char* nombre, char* comando, char* clave, char* valor){
	t_esi* esi = malloc(sizeof(t_esi));
	esi->nombre = strdup(nombre);
	esi->comando = strdup(comando);
	esi->clave = strdup(clave);
	esi->valor = strdup(valor);
	return esi;
}

void free_esi(t_esi* esi){
	free(esi->nombre);
	free(esi->comando);
	free(esi->clave);
	free(esi->valor);
	free(esi);
}
