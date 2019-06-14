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

void sendGossipingTable () {
    // todo Enviar tabla a kernel.
//    void print_gossiping_table(void* element) {
//        gossip_tad* gossip = element;
//        printf("IP: <%s> - Port: <%d> /n",gossip->IP, gossip->PORT);
//    }
//
//    list_iterate(LIST_GOSSIP, print_gossiping_table);
}