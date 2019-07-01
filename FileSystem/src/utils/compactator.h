#ifndef TP_2019_1C_GANK_MID_COMPACTATOR_H
#define TP_2019_1C_GANK_MID_COMPACTATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libio.h>
#include <math.h>
#include <pthread.h>

#include <commons/collections/list.h>
#include <commons/string.h>
#include <commonsfunctions/functions.h>
#include <serializador/estructuras.h>

#include "../setup/setup.h"
#include "../utils/utils.h"
#include "../utils/getterValuesFromFS.h"

pthread_t thread_compactation;
t_list * LIST_TABLE_COMPACTATION;

typedef struct compactation_table_tad {
    uint32_t numberPthread;
    create_tad* tableInfo;
} compactation_table_tad;

compactation_table_tad* new_compactation_table_tad(create_tad* tableInfo);
void free_compactation_table_tad(compactation_table_tad* compactationTable);
void init_list_compactation();
void runCompactation(char* table);
bool tmpToTmpc(char* path);
void liberarBloques(char* path);
void crearParticionCompactada(char* path, t_list* bloques, int size);
void eliminarTmpc(char* path);
void createThreadCompactation(char* nameTable, char* consistencia, u_int32_t particiones, u_int32_t compactacion);
void execCompactation();

#endif //TP_2019_1C_GANK_MID_COMPACTATOR_H
