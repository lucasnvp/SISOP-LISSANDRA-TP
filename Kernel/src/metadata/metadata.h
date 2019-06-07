//
// Created by utnso on 27/05/19.
//

#ifndef TP_2019_1C_GANK_MID_METADATA_H
#define TP_2019_1C_GANK_MID_METADATA_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <commons/collections/list.h>
#include <commons/string.h>
#include <serializador/estructuras.h>

#include "../criterios/criterios.h"

#define CRITERIO_SC "SC"
#define CRITERIO_SHC "SHC"
#define CRITERIO_EC "EC"

typedef struct {
    describe_tad* DESCRIBE;
    uint32_t MEMORY_NUMBER;
} metadata_tad;

t_list * LIST_METADATA;

void init_METADATA();
metadata_tad* new_metadata_tad(describe_tad* describe);
void free_metadata(metadata_tad* metadata);
void update_metadata_memory_number(metadata_tad* metadata, uint32_t number);
void load_METADATA(t_list* listDescribes);
void print_metadata();
metadata_tad* search_table(char* table);
bool exist_table(char* table);

#endif //TP_2019_1C_GANK_MID_METADATA_H
