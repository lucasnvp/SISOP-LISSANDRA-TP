//
// Created by utnso on 19/04/19.
//

#ifndef TP_2019_1C_GANK_MID_MEMORIA_H
#define TP_2019_1C_GANK_MID_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <serializador/estructuras.h>
#include <serializador/serializador.h>
#include <serializador/handshake.h>

t_log* log_Kernel_memory;

typedef struct {
    uint32_t MEMORY_NUMBER;
    char* IP_MEMORIA;
    uint32_t PUERTO_MEMORIA;
    uint32_t SOCKET;
    bool ACTIVA;
} memory_tad;

t_list * LIST_MEMORIES;

uint32_t RETARDO_GOSSIPING;

void init_memories();
void connect_memory(char* ip, uint32_t port);
memory_tad* memory_new(uint32_t number, char* ip, uint32_t port, uint32_t socket);
void add_memory(uint32_t number, char* ip, uint32_t port, uint32_t socket);
memory_tad* search_memory(uint32_t number);
bool exist_memory(uint32_t number);
void disable_memory(memory_tad* auxMemory);
void gossip_memory();
void gossip_memory_connect(t_list* gossip_table);
void compareGossipWithListMemory(gossip_tad* gossip);

#endif //TP_2019_1C_GANK_MID_MEMORIA_H
