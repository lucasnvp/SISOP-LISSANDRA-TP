//
// Created by utnso on 22/05/19.
//

#ifndef TP_2019_1C_GANK_MID_CRITERIOS_H
#define TP_2019_1C_GANK_MID_CRITERIOS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <commons/collections/list.h>
#include <commons/string.h>

#define CRITERIO_SC "SC"
#define CRITERIO_SHC "SHC"
#define CRITERIO_EC "EC"

typedef struct {
    uint32_t MEMORY_NUMBER;
    char* CRITERIO;
} criterio_tad;

t_list * LIST_CRITERIOS;

void init_criterios();
criterio_tad* criterio_new (uint32_t number, char* criterio);
bool criterio_add(uint32_t memoryNumber, char* criterio);

#endif //TP_2019_1C_GANK_MID_CRITERIOS_H
