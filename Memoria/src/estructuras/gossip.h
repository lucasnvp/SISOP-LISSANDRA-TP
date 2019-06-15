//
// Created by utnso on 12/06/19.
//

#ifndef TP_2019_1C_GANK_MID_GOSSIP_H
#define TP_2019_1C_GANK_MID_GOSSIP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <commons/collections/list.h>
#include <serializador/estructuras.h>
#include <serializador/serializador.h>

#include "../config/Config_memoria.h"

t_list * LIST_GOSSIP;

void inicializarTablaDeGossiping();
void funcionGossip();
void sendGossipingTable(uint32_t socket);

Type_Config config;

#endif //TP_2019_1C_GANK_MID_GOSSIP_H
