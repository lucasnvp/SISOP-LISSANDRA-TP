//
// Created by utnso on 22/05/19.
//

#include "criterios.h"

void init_criterios () {
    LIST_CRITERIOS = list_create();
}

criterio_tad* criterio_new (uint32_t number, char* criterio) {
    criterio_tad* c = malloc(sizeof(criterio_tad));
    c->MEMORY_NUMBER = number;
    c->CRITERIO = strdup(criterio);
    return c;
}

bool criterio_add (uint32_t memoryNumber, char* criterio) {

    bool allow = false;

    if(string_equals_ignore_case(criterio, CRITERIO_SC)){
        allow = exist_memory(memoryNumber);
    } else if(string_equals_ignore_case(criterio, CRITERIO_SHC)){
        allow = exist_memory(memoryNumber);
    } else if(string_equals_ignore_case(criterio, CRITERIO_EC)){
        allow = exist_memory(memoryNumber);
    } else {
        return allow;
    }

    if (allow){
        criterio_tad* c = criterio_new(memoryNumber, criterio);
        list_add(LIST_CRITERIOS, c);
    }

    return allow;

}