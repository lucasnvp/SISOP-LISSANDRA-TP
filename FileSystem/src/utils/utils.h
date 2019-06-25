#ifndef TP_2019_1C_GANK_MID_UTILS_H
#define TP_2019_1C_GANK_MID_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../comandos/comandos.h"
#include "../setup/setup.h"
#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <sys/time.h>

#define METADATA_OK true
#define METADATA_NO_OK false

static int CANTIDAD_TABLAS = 0;

void crear_carpeta(char* path);
void inicilizar_memtable();
char* settear_path_tabla(char* nombre_tabla);
void new_metadata_table(char* metadatabin, char* consistencia, char* particiones, char* compactacion);
t_config * obtener_metadata_table(char* metadatabin);
void mostrar_metadata_tabla(t_config * metadata, char* nombre_tabla);
void mostrar_metadatas(int requestOrigin);
void crear_metadata_table(char* nueva_tabla, char* consistencia, char* particiones, char* compactacion);
void asignar_bloques(char* path);
int bloque_libre();
int cantidad_bloquesLibres();
void crear_particiones(char* path, int cantidad_particiones);
char* crear_path_particion(char* path, int numero_particion);
void borrar_particion(char* path);
char** get_bloques_array(char* path);
describe_tad* crearDescribe(t_config* metadata, char* nombreTabla);

#endif //TP_2019_1C_GANK_MID_UTILS_H
