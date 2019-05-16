//
// Created by utnso on 16/05/19.
//

#include "memtableHanlder.h"

bool containsTable(char* nameKey) {
    return dictionary_has_key(memtable, nameKey);
}

char* getValue(char* table, int key) {

    if(containsTable(table)) {
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

            registro_tad * resultado = list_get(listaFiltrada,0);

            return resultado->value;
        } else {
            log_info(log_FileSystem, "No existe la key en la tabla: %i", key);
            printf("Se intento buscar una key no existente en la tabla: %i \n", key);
            return "error";
        }

    } else {
        log_info(log_FileSystem, "No existe la tabla en la mematble %s");
        printf("Se intento buscar en una tabla no existente en la memtable: %s \n", table);
        return "error";
    }
}

bool insertValue(char* table, registro_tad* registroTad) {
    t_list * list;

    if(!dictionary_has_key(memtable, table)) {
        list = list_create();
    } else {
        list = dictionary_get(memtable, table);
    }

    list_add(list, registroTad);
    dictionary_put(memtable, table, list);

    return true;
}