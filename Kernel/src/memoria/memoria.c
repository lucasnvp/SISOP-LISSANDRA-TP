//
// Created by utnso on 19/04/19.
//

#include "memoria.h"

void init_memories () {
    LIST_MEMORIES = list_create();
};

void connect_memory (char* ip, uint32_t port) {
    uint32_t SERVIDOR_MEMORIA = connect_server(ip, port);

    //Si conecto, informo
    if(SERVIDOR_MEMORIA > 1){
        log_info(log_Kernel_memory,"Connected successfully to the Memory");
        serializar_int(SERVIDOR_MEMORIA, NUEVA_CONEXION_KERNEL_TO_MEMORIA);
        memory_info_tad* memoryInfo = deserializar_memory_info(SERVIDOR_MEMORIA);
        RETARDO_GOSSIPING = memoryInfo->RETARDO_GOSSIPING;
        add_memory(memoryInfo->MEMORY_NUMBER, ip, port, SERVIDOR_MEMORIA);
        log_info(log_Kernel_memory, "Connected Memory Number: %d", memoryInfo->MEMORY_NUMBER);
        log_info(log_Kernel_memory, "Retardo del gossiping: %d", memoryInfo->RETARDO_GOSSIPING);
        free_memory_info_tad(memoryInfo);
    } else{
        log_info(log_Kernel_memory, "No se puedo conectar con la Memoria. IP <%s>", ip);
    }
}

memory_tad* memory_new (uint32_t number, char* ip, uint32_t port, uint32_t socket) {
    memory_tad* auxMemory = malloc(sizeof(memory_tad));
    auxMemory->MEMORY_NUMBER = number;
    auxMemory->IP_MEMORIA = strdup(ip);
    auxMemory->PUERTO_MEMORIA = port;
    auxMemory->SOCKET = socket;
    auxMemory->ACTIVA = true;
    return auxMemory;
}

void add_memory (uint32_t number, char* ip, uint32_t port, uint32_t socket) {
    memory_tad* auxMemory = memory_new(number, ip, port, socket);
    list_add(LIST_MEMORIES, auxMemory);
}

memory_tad* search_memory(uint32_t number){
    int _is_the_memory(memory_tad* m){
        return m->MEMORY_NUMBER == number;
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

void gossip_memory() {
    serializar_int(memory_ramdom_socket(), COMAND_GOSSIP);
}

uint32_t memory_ramdom_socket () {
    uint32_t listSize = list_size(LIST_MEMORIES);
    if (listSize == 0) {
        return -1;
    } else {
        uint32_t index = rand() % listSize;
        memory_tad* memory =  list_get(LIST_MEMORIES, index);
        return memory->SOCKET;
    }
}