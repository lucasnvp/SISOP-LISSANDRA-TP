//
// Created by utnso on 12/06/19.
//

#include "gossip.h"

void inicializarTablaDeGossiping(){
    LIST_GOSSIP = list_create();

    uint32_t index = 0;
    while(config->IP_SEEDS[index] != NULL){
        gossip_tad* gossip = new_gossip_tad(config->IP_SEEDS[index], atoi(config->PUERTO_SEEDS[index]));
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
            log_info(log_Memoria_gossip,"GOSSIP => Connected successfully <%s:%d>", gossip->IP, gossip->PORT);
            serializar_int(socket, COMAND_GOSSIP);
            t_list* listaGossip = deserializar_gossip_table(socket);
            compararTablasGossip(listaGossip);
            serializar_int(socket, COMAND_GOSSIP_RECEIVED);
            serializar_gossip_table(socket, LIST_GOSSIP);
            list_destroy_and_destroy_elements(listaGossip, free_gossip_tad);
            close(socket);
        } else{
            log_info(log_Memoria_gossip, "GOSSIP => Connect failed <%s:%d>", gossip->IP, gossip->PORT);
        }
    }

    t_list* listGD = list_duplicate(LIST_GOSSIP);
    list_remove(listGD, 0);
    list_iterate(listGD, gossip_element);
    list_destroy(listGD);
}

void sendGossipingTable (uint32_t socket) {
    serializar_gossip_table(socket, LIST_GOSSIP);
}

void compararTablasGossip(t_list* listaGossip) {
    void elementosAComparar(void* elemento) {
        gossip_tad* gossip = elemento;
        compararElementoEnTabla(gossip);
    }
    list_iterate(listaGossip,elementosAComparar);
}

void compararElementoEnTabla(gossip_tad* memoria) {
    bool addElement = true;
    void compararYAgregar(void* elemento) {
        gossip_tad* gossip = elemento;
        if(string_equals_ignore_case(memoria->IP, gossip->IP) && memoria->PORT == gossip->PORT) {
            addElement = false;
        }
    }
    list_iterate(LIST_GOSSIP, compararYAgregar);

    if (addElement) {
        gossip_tad* newGossip = new_gossip_tad(memoria->IP, memoria->PORT);
        list_add(LIST_GOSSIP, newGossip);
        log_info(log_Memoria_gossip, "GOSSIP => Se agrego la memoria <%s:%d>", newGossip->IP, newGossip->PORT);
    } else {
        log_info(log_Memoria_gossip, "GOSSIP => La memoria <%s:%d> ya se encuentra agregada", memoria->IP, memoria->PORT);
    }
}

void printGossip (uint32_t memoryNumber) {
    void printElement(void* elemento) {
        gossip_tad* gossip = elemento;
        log_info(log_Memoria_gossip, "Memory <%d> => <%s:%d>",memoryNumber, gossip->IP, gossip->PORT);
    }

    list_iterate(LIST_GOSSIP, printElement);
}