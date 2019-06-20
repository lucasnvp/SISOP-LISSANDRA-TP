//
// Created by utnso on 12/06/19.
//

#ifndef TP_2019_1C_GANK_MID_GOSSIP_H
#define TP_2019_1C_GANK_MID_GOSSIP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <servidor/servidor.h>
#include <serializador/estructuras.h>
#include <serializador/serializador.h>

#include "../config/Config_memoria.h"
#include "serializador/handshake.h"

t_log* log_Memoria_gossip;
t_list * LIST_GOSSIP;

void inicializarTablaDeGossiping();
void funcionGossip();
void sendGossipingTable(uint32_t socket);
void compararTablasGossip(t_list* listaGossip);
void compararElementoEnTabla(gossip_tad* memoria);
void printGossip(uint32_t memoryNumber);

Type_Config config;

#endif //TP_2019_1C_GANK_MID_GOSSIP_H
