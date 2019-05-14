//
// Created by utnso on 19/04/19.
//

#ifndef TP_2019_1C_GANK_MID_MEMORIA_H
#define TP_2019_1C_GANK_MID_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <commons/collections/list.h>

#include "../config/Config_kernel.h"

typedef struct {
    char* IP_MEMORIA;
    uint32_t PUERTO_MEMORIA;
    bool ACTIVA;
} memory_tad;

t_list * LIST_MEMORIES;

void init_memories(Type_Config* config);
memory_tad* memory_new(char* ip, uint32_t port);

#endif //TP_2019_1C_GANK_MID_MEMORIA_H
