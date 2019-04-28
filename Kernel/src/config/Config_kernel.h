//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_CONFIG_KERNEL_H
#define TP_2019_1C_GANK_MID_CONFIG_KERNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <commons/string.h>
#include "commons/config.h"
#include "commons/log.h"

typedef struct ConfigType{
    char* IP_MEMORIA;
    uint32_t PUERTO_MEMORIA;
    uint32_t QUANTUM;
    uint32_t MULTIPROCESAMIENTO;
    uint32_t REFRESH_METADATA;
    uint32_t RETARDO;
} Type_Config;

Type_Config* load_config(char* path);
void print_config(Type_Config* auxConfig, t_log* log_Console);
void free_config(Type_Config* config);

#endif //TP_2019_1C_GANK_MID_CONFIG_KERNEL_H
