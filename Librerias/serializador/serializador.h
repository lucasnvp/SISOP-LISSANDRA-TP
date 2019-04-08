#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "../servidor/servidor.h"
#include "estructuras.h"

typedef struct {
	char* data;
	int size;
} t_stream;

int i, j;

t_stream* stream_create(int size);
void stream_destroy(t_stream* stream);

void serializar_int(uint32_t socket, uint32_t number);
uint32_t deserializar_int(uint32_t socket);

void serializar_string(int client, char* stringToSend);
char* deserializar_string(int servidor);

void serializar_esi(uint32_t socket, t_esi* esi);
t_esi* deserializar_esi(uint32_t socket);

#endif /* SERIALIZADOR_H_ */
