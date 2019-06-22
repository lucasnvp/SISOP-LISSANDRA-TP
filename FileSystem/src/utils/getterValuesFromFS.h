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

registro_tad* getValueFromTemporalFile(char *table, int key, char *typeFile);
registro_tad* getValueFromPartition(char *table, int key, char* typeFile, uint32_t partition);
char* getRegistersFromBinaryFile(char *pathTabla);
char* leerBloque(char* nroBloque, int size);
t_list* transformRegistersStrToStructs(char* strRegs);
registro_tad* obtenerRegistroSegunKey(t_list* registros, int key);
registro_tad* verifyMaxValue(registro_tad* finalResult, registro_tad *registerFrom);
t_list* getRegistersFromTemporals(char* table, char* typeFile);
t_list* getRegistersFromPartition(char* table, char* typeFile, uint32_t partition);

#endif //TP_2019_1C_GANK_MID_GETTERVALUESFROMFS_H
