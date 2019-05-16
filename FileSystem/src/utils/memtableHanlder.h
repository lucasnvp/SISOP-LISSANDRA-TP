//
// Created by utnso on 16/05/19.
//

#ifndef TP_2019_1C_GANK_MID_MEMTABLEHANLDER_H
#define TP_2019_1C_GANK_MID_MEMTABLEHANLDER_H

#include "commons/collections/dictionary.h"
#include "commons/collections/list.h"
#include "stdbool.h"
#include "commons/log.h"
#include "commonsfunctions/functions.h"
#include "../setup/setup.h"
#include "../utils/utils.h"
#include "stdlib.h"
#include "string.h"

t_dictionary * memtable;

bool containsTable(char *nameKey);
char* getValue(char* table, int key);
bool insertValue(char* table, registro_tad* registroTad);


#endif //TP_2019_1C_GANK_MID_MEMTABLEHANLDER_H
