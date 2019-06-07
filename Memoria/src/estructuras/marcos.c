//
// Created by utnso on 16/05/19.
//

#include "marcos.h"

// Reserva un marco para una Página marcándolo como ocupado y devuelve su dirección de memoria
// Si todos los marcos se encuentran ocupados, se buscará una Página para Liberar

void liberarMarco(registro_tad* punteroAPagina){

    uint32_t numeroDeMarco = (uint32_t) ((punteroAPagina - (registro_tad*) memoriaPrincipal) );

    struct tablaDeMarcos* _tablaDeMarcos = primerRegistroDeMarcos;
    while(_tablaDeMarcos != NULL){
        if(_tablaDeMarcos->registro.numeroMarco == numeroDeMarco){
            _tablaDeMarcos->registro.marcoOcupado = false;
            return;
        }
        _tablaDeMarcos = _tablaDeMarcos->siguiente;
    }

}

registro_tad* reservarMarco() {
    struct tablaDeMarcos* _tablaDeMarcos = primerRegistroDeMarcos;
    while(_tablaDeMarcos != NULL){
        if(_tablaDeMarcos->registro.marcoOcupado == false) {
            _tablaDeMarcos->registro.marcoOcupado = true;
            return (registro_tad*)(memoriaPrincipal + _tablaDeMarcos->registro.numeroMarco * sizeof(registro_tad));
        }


        _tablaDeMarcos = _tablaDeMarcos->siguiente;
    }
    // si no se encontró ningun marco libre, se procede a liberar una página
    return liberarPagina();
}

void inicializarMarcos(uint32_t tamanioMemoria){
    //calculo la cantidad de marcos
    cantidadDeMarcos = tamanioMemoria / sizeof(registro_tad);
    struct tablaDeMarcos* primerRegistroDeMarco = malloc(sizeof(tablaDeMarcos));
    primerRegistroDeMarco->registro.numeroMarco = 0;
    primerRegistroDeMarco->registro.marcoOcupado = false;
    primerRegistroDeMarcos = primerRegistroDeMarco;
    struct tablaDeMarcos* aux = primerRegistroDeMarco;
    for(int index = 1; index < cantidadDeMarcos; index++){
        struct tablaDeMarcos* registroDeMarco = malloc(sizeof(tablaDeMarcos));
        registroDeMarco->registro.numeroMarco = index;
        registroDeMarco->registro.marcoOcupado = false;
        aux->siguiente = registroDeMarco;
        aux = registroDeMarco;
    }
    aux->siguiente = NULL;
    return;
}
