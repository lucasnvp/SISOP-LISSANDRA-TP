//
// Created by utnso on 16/05/19.
//

#include "marcos.h"

// Reserva un marco para una Página marcándolo como ocupado y devuelve su dirección de memoria
// Si todos los marcos se encuentran ocupados, se buscará una Página para Liberar

registo_tad* reservarMarco() {
    struct tablaDeMarcos* _tablaDeMarcos = primerMarco;
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
