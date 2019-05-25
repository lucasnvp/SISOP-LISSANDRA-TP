//
// Created by utnso on 19/04/19.
//

#include "memoria.h"
#include "../config/Config_kernel.h"

void init_memories (Type_Config* config) {
    LIST_MEMORIES = list_create();
    add_memory(1, config->IP_MEMORIA, config->PUERTO_MEMORIA);
};

memory_tad* memory_new (uint32_t number, char* ip, uint32_t port) {
    memory_tad* auxMemory = malloc(sizeof(auxMemory));
    auxMemory->MEMORY_NUMBER = number;
    auxMemory->IP_MEMORIA = strdup(ip);
    auxMemory->PUERTO_MEMORIA = port;
    auxMemory->ACTIVA = true;
    return auxMemory;
}

void add_memory (uint32_t number, char* ip, uint32_t port) {
    memory_tad* auxMemory = memory_new(number, ip, port);
    list_add(LIST_MEMORIES, auxMemory);
}

memory_tad* search_memory(uint32_t number){
    int _is_the_memory(memory_tad* m){
        return m->MEMORY_NUMBER = number;
    }

    memory_tad* auxMemory = list_find(LIST_MEMORIES, (void*) _is_the_memory);

    return auxMemory;
}

bool exist_memory (uint32_t number) {
    memory_tad* auxMemory = search_memory(number);

    if (auxMemory == NULL || auxMemory->ACTIVA == false) {
        return false;
    } else {
        return true;
    }
}

void disable_memory(memory_tad* auxMemory){
    auxMemory->ACTIVA = false;
}