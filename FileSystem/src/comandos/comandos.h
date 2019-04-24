//
// Created by utnso on 08/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

#include "commons/log.h"
#include "commonsfunctions/functions.h"
#include "serializador/estructuras.h"
#include "../setup/setup.h"
#include "../utils/utils.h"
#include "stdlib.h"
#include "string.h"

t_log* log_FileSystem;
t_dictionary * memtable;

void print_console(void (*log_function)(t_log*, const char*), char* message);
void comando_select();
void comando_insert(char* table, int key, char* value, int timestamp);
void comando_create(char* _table, char* consistencia, char* cantidad_particiones, char* compactacion);
void comando_describe_all();
void comando_describe(char* nombre_tabla);
void comando_drop(char* nombre_tabla);

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
