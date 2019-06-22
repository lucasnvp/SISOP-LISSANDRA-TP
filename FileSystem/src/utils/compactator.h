#ifndef TP_2019_1C_GANK_MID_COMPACTATOR_H
#define TP_2019_1C_GANK_MID_COMPACTATOR_H

#include <commons/collections/list.h>
#include <libio.h>
#include <stdio.h>
#include <stdlib.h>
#include <serializador/estructuras.h>
#include <commons/string.h>
#include "../setup/setup.h"
#include <math.h>
#include "../utils/utils.h"
#include "../utils/getterValuesFromFS.h"
#include <commonsfunctions/functions.h>

void runCompactation(char* table);
bool tmpToTmpc(char* path);
void liberarBloques(char* path);
void crearParticionCompactada(char* path, t_list* bloques, int size);
void eliminarTmpc(char* path);

#endif //TP_2019_1C_GANK_MID_COMPACTATOR_H
