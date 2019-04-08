//
// Created by utnso on 07/04/19.
//

#ifndef TP_2019_1C_GANK_MID_CONFIG_FILESYSTEM_H
#define TP_2019_1C_GANK_MID_CONFIG_FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <commons/string.h>
#include "commons/config.h"
#include "commons/log.h"

typedef struct ConfigType{
    uint32_t PUERTO_ESCUCHA;
    uint32_t CANT_CONEXIONES;
    char* PUNTO_MONTAJE;
    uint32_t RETARDO;
    uint32_t TAMANO_VALUE;
    uint32_t TIEMPO_DUMP;
} Type_Config;

Type_Config load_config(char* path);
void print_config(Type_Config auxConfig, t_log* log_Console);

#endif //TP_2019_1C_GANK_MID_CONFIG_FILESYSTEM_H
