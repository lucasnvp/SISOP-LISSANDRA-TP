#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct registro_tad{
    uint32_t timestamp;
    uint32_t key;
    char* value;
}registo_tad;

registo_tad* new_registro_tad(uint32_t timestamp, uint32_t key, char* value);
void free_registro_tad(registo_tad* registro);

typedef struct ESIType{
	char* nombre;
	char* comando;
	char* clave;
	char* valor;
}t_esi;

t_esi* new_esi(char* nombre, char* comando, char* clave, char* valor);
void free_esi(t_esi* esi);

#endif /* ESTRUCTURAS_H_ */
