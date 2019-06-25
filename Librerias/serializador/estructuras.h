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
}registro_tad;

registro_tad* new_registro_tad(uint32_t timestamp, uint32_t key, char* value);
void free_registro_tad(registro_tad* registro);

typedef struct insert_tad{
    char* nameTable;
    uint32_t key;
    char* value;
}insert_tad;

insert_tad* new_insert_tad(char* nameTable, uint32_t key, char* value);
void free_insert_tad(insert_tad* insert);

typedef struct select_tad{
    char* nameTable;
    uint32_t key;
}select_tad;

select_tad* new_select_tad(char* nameTable, uint32_t key);
void free_select_tad(select_tad* select);

struct table_tad {
    char* nameTable;
    char* consistencia;
    uint32_t particiones;
    uint32_t compactacion;
};

typedef struct table_tad create_tad;
create_tad* new_create_tad(char* nameTable, char* consistencia, u_int32_t particiones, u_int32_t compactacion);
void free_create_tad(create_tad* create);

typedef struct table_tad describe_tad;
describe_tad* new_describe_tad(char* nameTable, char* consistencia, u_int32_t particiones, u_int32_t compactacion);
void free_describe_tad(describe_tad* create);

typedef struct memory_info_tad {
    uint32_t MEMORY_NUMBER;
    uint32_t RETARDO_GOSSIPING;
}memory_info_tad;

memory_info_tad* new_memory_info_tad(uint32_t number, uint32_t delayGossip);
void free_memory_info_tad(memory_info_tad* memoryInfo);

typedef struct gossip_tad{
    char* IP;
    uint32_t PORT;
} gossip_tad;

gossip_tad* new_gossip_tad(char* ip, uint32_t port);
void free_gossip_tad(gossip_tad* gossip);

#endif /* ESTRUCTURAS_H_ */
