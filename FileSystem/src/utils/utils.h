#ifndef TP_2019_1C_GANK_MID_UTILS_H
#define TP_2019_1C_GANK_MID_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "commons/string.h"
#include "../comandos/comandos.h"
#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/string.h>

static const char* CONSISTENCY = "CONSISTENCY=SC\n";
static const char* PARTITIONS = "PARTITIONS=4\n";
static const char* COMPACTATION_TIME = "COMPACTATION_TIME=60000\n";

t_config * metadataTableConfig;

void crear_carpeta(char* path);
char* settear_path_tabla(char* nombre_tabla);
void new_metadata_table(char* metadatabin);
void metadata_table_setup(char* metadatabin);
void crear_metadata_table(char* nueva_tabla);

#endif //TP_2019_1C_GANK_MID_UTILS_H
