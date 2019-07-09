//
// Created by utnso on 16/05/19.
//

#ifndef TP_2019_1C_GANK_MID_MEMTABLEHANLDER_H
#define TP_2019_1C_GANK_MID_MEMTABLEHANLDER_H

#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include <semaphore.h>

#include "commons/collections/dictionary.h"
#include "commons/collections/list.h"
#include "commons/log.h"
#include "commonsfunctions/functions.h"

#include "../setup/setup.h"
#include "../utils/utils.h"
#include "../utils/semaphores.h"

t_dictionary * memtable;

bool containsTable(char *nameKey);
registro_tad* getValueFromMemtable(char *table, int key);
void insertValue(char* table, registro_tad* registroTad);
t_list* getListOfReg(char* table);


#endif //TP_2019_1C_GANK_MID_MEMTABLEHANLDER_H
