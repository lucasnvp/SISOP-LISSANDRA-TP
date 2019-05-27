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

#include "../criterios/criterios.h"

typedef struct {
    char* TABLA;
    char* CRITERIO;
    uint32_t KEY;
} metadata_tad;

t_list * LIST_METADATA;

void init_METADATA();

#endif //TP_2019_1C_GANK_MID_METADATA_H
