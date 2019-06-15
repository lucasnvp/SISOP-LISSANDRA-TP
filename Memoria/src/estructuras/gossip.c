//
// Created by utnso on 12/06/19.
//

#include "gossip.h"

void inicializarTablaDeGossiping(){
    LIST_GOSSIP = list_create();

    uint32_t index = 0;
    while(config.IP_SEEDS[index] != NULL){
        gossip_tad* gossip = new_gossip_tad(config.IP_SEEDS[index], atoi(config.PUERTO_SEEDS[index]));
        list_add(LIST_GOSSIP, gossip);
        index++;
    }
}

void funcionGossip(){

}

void sendGossipingTable (uint32_t socket) {
    serializar_gossip_table(socket, LIST_GOSSIP);
}