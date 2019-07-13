//
// Created by utnso on 16/05/19.
//

#include "memtableHanlder.h"

bool containsTable(char* nameKey) {
    pthread_mutex_lock(&SEM_MEMTABLE);
    bool exist = dictionary_has_key(memtable, nameKey);
    pthread_mutex_unlock(&SEM_MEMTABLE);

    return exist;
}

registro_tad* getValueFromMemtable(char *table, int key) {


    bool contains = containsTable(table);

    if(contains == true) {

        pthread_mutex_lock(&SEM_MEMTABLE);
        t_list* list = dictionary_get(memtable, table);

        bool _mismaKey(registro_tad* registro) {
            return registro->key == key;
        }

        t_list * listaFiltrada = list_filter(list, (void*) _mismaKey);

        if(listaFiltrada->elements_count > 0) {
            bool _timestampMayor(registro_tad * primera, registro_tad * segunda) {
                return primera->timestamp > segunda->timestamp;
            }

            list_sort(listaFiltrada, (void*) _timestampMayor);

            list_get(listaFiltrada,0);

        } else {
            pthread_mutex_unlock(&SEM_MEMTABLE);
            return NULL;
        }
        pthread_mutex_unlock(&SEM_MEMTABLE);
    } else {
        return NULL;
    }
}

void insertValue(char* table, registro_tad* registroTad) {
    pthread_mutex_lock(&SEM_MEMTABLE);
    t_list * list;
    if(!dictionary_has_key(memtable, table)) {
        list = list_create();
    } else {
        list = dictionary_get(memtable, table);
    }

    list_add(list, registroTad);
    dictionary_put(memtable, table, list);
    pthread_mutex_unlock(&SEM_MEMTABLE);
}

// DEPRECATED
t_list* getListOfReg(char* table){

    if(containsTable(table)){

        printf("Se encontro la tabla %s en la memtable\n", table);

        t_list * registros = list_create();
        registros = dictionary_get(memtable,table);

        return registros;
    }

    return NULL;
}