#ifndef TP_2019_1C_GANK_MID_UTILS_H
#define TP_2019_1C_GANK_MID_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "commons/string.h"
#include "../comandos/comandos.h"
#include "../setup/setup.h"
#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/string.h>

static int CANTIDAD_TABLAS = 0;

void crear_carpeta(char* path);
void inicilizar_memtable();
char* settear_path_tabla(char* nombre_tabla);
void new_metadata_table(char* metadatabin, char* consistencia, char* particiones, char* compactacion);
t_config * obtener_metadata_table(char* metadatabin);
void mostrar_metadata_tabla(t_config * metadata, char* nombre_tabla);
void mostrar_metadatas();
void crear_metadata_table(char* nueva_tabla, char* consistencia, char* particiones, char* compactacion);
void asignar_bloques(char* path);
int bloque_libre();
void cantidad_bloquesLibres();
void crear_particiones(char* path, int cantidad_particiones);
char* crear_path_particion(char* path, int numero_particion);
void borrar_particion(char* path);
char** get_bloques_array(char* path);

#endif //TP_2019_1C_GANK_MID_UTILS_H
