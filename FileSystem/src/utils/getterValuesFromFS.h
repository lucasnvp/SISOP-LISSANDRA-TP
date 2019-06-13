//
// Created by utnso on 02/06/19.
//

#ifndef TP_2019_1C_GANK_MID_GETTERVALUESFROMFS_H
#define TP_2019_1C_GANK_MID_GETTERVALUESFROMFS_H

#include <commons/collections/list.h>
#include <libio.h>
#include <stdio.h>
#include <stdlib.h>
#include <serializador/estructuras.h>
#include <commons/string.h>
#include "../setup/setup.h"
#include <math.h>
#include "../utils/utils.h"
#include <commonsfunctions/functions.h>

registro_tad* getValueFromTemporals(char* table, int key);
char* getRegistrosConcatenadosDeUnTmp(char* pathTabla);
char* leerBloque(char* nroBloque, int size);
t_list* transformRegistersStrToStructs(char* strRegs);
registro_tad* obtenerRegistroSegunKey(t_list* registros, int key);

#endif //TP_2019_1C_GANK_MID_GETTERVALUESFROMFS_H
