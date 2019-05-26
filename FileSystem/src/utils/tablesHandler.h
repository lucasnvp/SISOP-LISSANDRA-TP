#ifndef TP_2019_1C_GANK_MID_TABLESHANDLER_H
#define TP_2019_1C_GANK_MID_TABLESHANDLER_H

#include <commons/collections/list.h>
#include <stdio.h>
#include <serializador/estructuras.h>
#include <commons/string.h>
#include "../setup/setup.h"
#include <math.h>
#include "../utils/utils.h"
#include <commonsfunctions/functions.h>

void _crearTmp(char* nombreTabla, t_list * registros);
int getSizeRegistroTad(registro_tad* registro);
int getBloquesParaRegistro(registro_tad *registro);
void crearArchivoTemporal(char* nombreTabla, t_list* bloques);
char* crear_path_tmp(char* tabla, int dump);
int getBloquesNecesarios(t_list* registros, t_list* bloquesAOcupar);
#endif //TP_2019_1C_GANK_MID_TABLESHANDLER_H
