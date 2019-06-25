#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <commons/collections/list.h>

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

t_stream* serializar_table(struct table_tad* table);
struct table_tad* deserializar_table(char* stream, int* size);

void serializar_create(uint32_t socket, create_tad* create);
create_tad* deserializar_create(uint32_t socket);

void serializar_describe(uint32_t socket, describe_tad* describe);
describe_tad* deserializar_describe(uint32_t socket);

void serializar_describe_all(uint32_t socket, t_list* describe_all);
t_list* deserializar_describe_all(uint32_t socket);

void serializar_memory_info(uint32_t socket, memory_info_tad* memory);
memory_info_tad* deserializar_memory_info(uint32_t socket);

t_stream* serializar_gossip(gossip_tad* gossip);
gossip_tad* deserializar_gossip(char* stream, int* size);

void serializar_gossip_table(uint32_t socket, t_list* gossip);
t_list* deserializar_gossip_table(uint32_t socket);

#endif /* SERIALIZADOR_H_ */
