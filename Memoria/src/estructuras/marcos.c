//
// Created by utnso on 16/05/19.
//

#include "marcos.h"

// Reserva un marco para una Página marcándolo como ocupado y devuelve su dirección de memoria
// Si todos los marcos se encuentran ocupados, se buscará una Página para Liberar

registo_tad* reservarMarco() {
    struct tablaDeMarcos* _tablaDeMarcos = primerRegistroDeMarcos;
    while(_tablaDeMarcos != NULL){
        if(_tablaDeMarcos->registro.marcoOcupado == false) {
            _tablaDeMarcos->registro.marcoOcupado = true;
            return (registo_tad*)(memoriaPrincipal + _tablaDeMarcos->registro.numeroMarco * sizeof(registo_tad));
        }


        _tablaDeMarcos = _tablaDeMarcos->siguiente;
    }
    // si no se encontró ningun marco libre, se procede a liberar una página
    return liberarPagina();
}

void inicializarMarcos(uint32_t tamanioMemoria){
    //calculo la cantidad de marcos
    cantidadDeMarcos = tamanioMemoria / sizeof(registo_tad);
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
