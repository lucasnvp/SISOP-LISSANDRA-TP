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

typedef struct create_tad{
    char* nameTable;
    char* consistencia;
    u_int32_t particiones;
    u_int32_t compactacion;
}create_tad;

create_tad* new_create_tad(char* nameTable, char* consistencia, u_int32_t particiones, u_int32_t compactacion);
void free_create_tad(create_tad* create);

typedef struct describe_tad{
    char* nameTable;
    char* consistencia;
    u_int32_t particiones;
    u_int32_t compactacion;
}describe_tad;

describe_tad* new_describe_tad(char* nameTable, char* consistencia, u_int32_t particiones, u_int32_t compactacion);
void free_create_tad(describe_tad* create);

#endif /* ESTRUCTURAS_H_ */
