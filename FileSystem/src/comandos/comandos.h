//
// Created by utnso on 08/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

#include "serializador/estructuras.h"
#include "serializador/serializador.h"
#include "stdlib.h"
#include "string.h"
#include "commons/log.h"
#include "commonsfunctions/functions.h"
#include "commons/collections/dictionary.h"
#include "commons/collections/list.h"
#include "../setup/setup.h"
#include "../utils/utils.h"
#include "../utils/memtableHanlder.h"
#include "../utils/tablesHandler.h"
#include "../utils/getterValuesFromFS.h"
#include "../utils/compactator.h"

// Flags que indican quien ejecuta el comando
#define CONSOLE_REQUEST	-1
#define NOT_TIMESTAMP -1

#define NO_EXISTE_TABLA false
#define YA_EXISTE_TABLA true
#define INSERT_OK true
#define CREATE_OK true
#define DESCRIBE_OK true

t_log* log_FileSystem;

void print_console(void (*log_function)(t_log*, const char*), char* message);
void comando_select(char* table, int key, int requestOrigin);
void comando_insert(char* table, int key, char* value, int timestamp, int requestOrigin);
void comando_create(char* _table, char* consistencia, char* cantidad_particiones, char* compactacion, int requestOrigin);
void comando_describe_all(int requestOrigin);
void comando_describe(char* nombre_tabla, int requestOrigin);
void comando_drop(char* nombre_tabla, int requestOrigin);
void comando_dump();
void comando_compactation(char* table);

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
