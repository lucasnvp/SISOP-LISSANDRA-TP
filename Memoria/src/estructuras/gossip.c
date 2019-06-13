//
// Created by utnso on 12/06/19.
//

#include "gossip.h"



void inicializarTablaDeGossiping(){
    uint32_t index = 0;
    struct tablaDeGossip* registroAnterior;
    while(config.IP_SEEDS[index] != NULL){
        if(index == 0){
            struct tablaDeGossip* primerRegistro = malloc(sizeof( tablaDeGossip));
            primerRegistro->registro.numeroMemoria = config.MEMORY_NUMBER;
            //primerRegistro->registro.ip =
            primerRegistro->registro.puerto = config.PUERTO_ESCUCHA;
            primerRegistroDeGossip = primerRegistro;
            registroAnterior = primerRegistro;
        }
        struct tablaDeGossip* nuevoRegistro = malloc(sizeof( tablaDeGossip));
        nuevoRegistro->registro.numeroMemoria = MEMORIA_NO_CONECTADA;
        nuevoRegistro->registro.ip = config.IP_SEEDS[index];
        nuevoRegistro->registro.puerto = atoi( config.PUERTO_SEEDS[index] );
        nuevoRegistro->siguiente = NULL;
        registroAnterior->siguiente = nuevoRegistro;
        registroAnterior = nuevoRegistro;
        index++;
    }
}

void funcionGossip(){
    struct tablaDeGossip* _TablaDeGossip = primerRegistroDeGossip->siguiente;

    while(_TablaDeGossip != NULL){

        if(_TablaDeGossip->registro.numeroMemoria != MEMORIA_NO_CONECTADA){

        }

        _TablaDeGossip = _TablaDeGossip->siguiente;

    }
}
