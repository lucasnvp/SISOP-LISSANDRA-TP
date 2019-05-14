//
// Created by utnso on 19/04/19.
//

#include "memoria.h"
#include "../config/Config_kernel.h"

void init_memories(Type_Config* config) {
    LIST_MEMORIES = list_create();
    memory_tad* memory = memory_new(config->IP_MEMORIA, config->PUERTO_MEMORIA);
    list_add(LIST_MEMORIES, memory);
};

memory_tad* memory_new(char* ip, uint32_t port){
    memory_tad* auxMemory = malloc(sizeof(auxMemory));
    auxMemory->IP_MEMORIA = strdup(ip);
    auxMemory->PUERTO_MEMORIA = port;
    auxMemory->ACTIVA = true;
    return auxMemory;
}