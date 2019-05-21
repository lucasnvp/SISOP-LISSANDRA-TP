//
// Created by utnso on 19/04/19.
//

#include "memoria.h"
#include "../config/Config_kernel.h"

void init_memories (Type_Config* config) {
    LIST_MEMORIES = list_create();
    memory_tad* memory = memory_new(1, config->IP_MEMORIA, config->PUERTO_MEMORIA);
    list_add(LIST_MEMORIES, memory);
};

memory_tad* memory_new (uint32_t number, char* ip, uint32_t port) {
    memory_tad* auxMemory = malloc(sizeof(auxMemory));
    auxMemory->MEMORY_NUMBER = number;
    auxMemory->IP_MEMORIA = strdup(ip);
    auxMemory->PUERTO_MEMORIA = port;
    auxMemory->ACTIVA = true;
    return auxMemory;
}

bool exist_memory (uint32_t number) {
    int _is_the_memory(memory_tad* m){
        return m->MEMORY_NUMBER = number;
    }

    memory_tad* auxMemory = list_find(LIST_MEMORIES, (void*) _is_the_memory);

    if (auxMemory == NULL) {
        return false;
    } else {
        return true;
    }

}