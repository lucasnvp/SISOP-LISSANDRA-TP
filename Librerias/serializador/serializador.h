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

void serializar_registro(uint32_t socket, registro_tad* registro);
registro_tad* deserializar_registro(uint32_t socket);

void serializar_insert(uint32_t socket, insert_tad* insert);
insert_tad* deserializar_insert(uint32_t socket);

void serializar_select(uint32_t socket, select_tad* select);
select_tad* deserializar_select(uint32_t socket);

void serializar_create(uint32_t socket, create_tad* create);
create_tad* deserializar_create(uint32_t socket);

#endif /* SERIALIZADOR_H_ */
