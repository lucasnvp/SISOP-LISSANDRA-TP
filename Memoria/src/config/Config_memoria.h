//
// Created by utnso on 07/04/19.
//

#ifndef TP_2019_1C_GANK_MID_CONFIG_MEMORIA_H
#define TP_2019_1C_GANK_MID_CONFIG_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <commons/string.h>
#include "commons/config.h"
#include "commons/log.h"

typedef struct ConfigType{
    uint32_t PUERTO_ESCUCHA;
    char* IP_FS;
    uint32_t PUERTO_FS;
    char** IP_SEEDS;
    char** PUERTO_SEEDS;
    uint32_t RETARDO_MEM;
    uint32_t RETARDO_FS;
    uint32_t TAM_MEM;
    uint32_t RETARDO_JOURNAL;
    uint32_t RETARDO_GOSSIPING;
    uint32_t MEMORY_NUMBER;
} Type_Config;

int i;

Type_Config load_config(char* path);
void print_config(Type_Config auxConfig, t_log* log_Console);

#endif //TP_2019_1C_GANK_MID_CONFIG_MEMORIA_H
