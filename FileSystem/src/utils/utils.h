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
void new_metadata_table(char* metadatabin, char* consistencia, char* particiones, char* compactacion);
void metadata_table_setup(char* metadatabin);
void crear_metadata_table(char* nueva_tabla, char* consistencia, char* particiones, char* compactacion);
void asignar_bloques(char* path);
int32_t bloque_libre(t_bitarray* auxBitArray);
void crear_particiones(char* path, int cantidad_particiones);
char* crear_path_particion(char* path, int numero_particion);
void borrar_particion(char* path);
char** get_bloques_array(char* path);

#endif //TP_2019_1C_GANK_MID_UTILS_H
