//
// Created by utnso on 08/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

#include "commons/log.h"
#include "commonsfunctions/functions.h"
#include "../setup/setup.h"
#include "../utils/utils.h"
#include "stdlib.h"

t_log* log_FileSystem;

void print_console(void (*log_function)(t_log*, const char*), char* message);
void comando_select();
void comando_insert();
void comando_create(char* _table, char* consistencia, char* cantidad_particiones, char* compactacion);
void comando_describe();
void comando_drop();

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
