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
#include <commons/log.h>
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
void create_and_add(t_list* listDescribes, t_list* listToAdd);
void set_previous_memory_number(metadata_tad* metadata);
void print_metadata(t_log* log);
metadata_tad* search_table(char* table);
uint32_t get_memory_socket_from_metadata(char* table);

#endif //TP_2019_1C_GANK_MID_METADATA_H
