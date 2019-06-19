//
// Created by utnso on 12/06/19.
//

#include "gossip.h"

void inicializarTablaDeGossiping(){
    LIST_GOSSIP = list_create();

    uint32_t index = 1;
    while(config.IP_SEEDS[index] != NULL){
        gossip_tad* gossip = new_gossip_tad(config.IP_SEEDS[index], atoi(config.PUERTO_SEEDS[index]));
        list_add(LIST_GOSSIP, gossip);
        index++;
    }
}

void funcionGossip(){
    log_info(log_Memoria_gossip,"GOSSIP");

    void gossip_element(void* element){
        gossip_tad* gossip = element;
        uint32_t socket = connect_server(gossip->IP,gossip->PORT);
        if(socket > 1){
            log_info(log_Memoria_gossip,"GOSSIP => Connected successfully <%s> <%d>", gossip->IP, gossip->PORT);
            // Pedir tabla
            // Crear una funcion para Comparar y agregar a la lista
            // desconectar
        } else{
            log_info(log_Memoria_gossip, "GOSSIP => Connect failed <%s> <%d>", gossip->IP, gossip->PORT);
        }
    }

    list_iterate(LIST_GOSSIP, gossip_element);

}

void sendGossipingTable (uint32_t socket) {
    serializar_gossip_table(socket, LIST_GOSSIP);
}