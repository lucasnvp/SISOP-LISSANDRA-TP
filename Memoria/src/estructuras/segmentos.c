//
// Created by utnso on 13/05/19.
//

#include "segmentos.h"

//busca en los segmentos, si lo encuentra lo retorna,
// si no lo encuentra, lo crea

tablaDeSegmentos* buscarSegmento(char* nombreDeTabla){
    struct tablaDeSegmentos* _TablaDeSegmento;
    _TablaDeSegmento = primerRegistroDeSegmentos;

    while(_TablaDeSegmento != NULL){
        if(strcmp(_TablaDeSegmento->registro.nombreTabla, nombreDeTabla) == 0){
            return _TablaDeSegmento;
        }
        _TablaDeSegmento = _TablaDeSegmento->siguiente;
    }

    return NULL;
}

// Agrega un segmento a la tabla de segmentos
tablaDeSegmentos* agregarSegmento(char* nombreDeTabla){
    struct tablaDeSegmentos* _TablaDeSegmento;
    _TablaDeSegmento = primerRegistroDeSegmentos;
    struct tablaDeSegmentos* ultimo = NULL;

    struct tablaDeSegmentos* nuevoRegistroSegmento = malloc(sizeof(tablaDeSegmentos));
    // en tanto la tabla de segmentos no sea nula, itero
    while(_TablaDeSegmento != NULL){
        ultimo = _TablaDeSegmento;
        _TablaDeSegmento = _TablaDeSegmento->siguiente;
    }

    if(ultimo==NULL){ //por si aun no fue creado ningur registro de segmento
        primerRegistroDeSegmentos = nuevoRegistroSegmento;
        nuevoRegistroSegmento->registro.idSegmento = (uint32_t) 1;
    } else {
        ultimo->siguiente = nuevoRegistroSegmento;
        nuevoRegistroSegmento->registro.idSegmento = ultimo->registro.idSegmento + 1;

    }

    nuevoRegistroSegmento->siguiente = NULL;

    nuevoRegistroSegmento->registro.nombreTabla = malloc(strlen(nombreDeTabla) + 1);
    strcpy(nuevoRegistroSegmento->registro.nombreTabla, nombreDeTabla);

    nuevoRegistroSegmento->registro.tablaDePaginas = NULL;

    return nuevoRegistroSegmento;
}


void reenlazarSegmentos(tablaDeSegmentos* tablaDeSegmentos) {
// tablaDeSegmentos es el segmento a liberar
    if( primerRegistroDeSegmentos == tablaDeSegmentos) {
        // si es el primer registro de segmentos, le asino al inicio de la lista el siguiente
       primerRegistroDeSegmentos = tablaDeSegmentos->siguiente;
    } else {
        // si no lo es, asigno el siguiente del primer registro de segmento
        struct tablaDeSegmentos* _tablaDeSegmentos = primerRegistroDeSegmentos;
        while(_tablaDeSegmentos != NULL) {
            // si encuentro la tabla que estoy buscando para liberar
            if(tablaDeSegmentos == _tablaDeSegmentos->siguiente) {
                //reenlazo (en actualizarIdSegmentos libero porque necesito la referencia)
                _tablaDeSegmentos->siguiente = tablaDeSegmentos->siguiente;
                return;
            }
            _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
        }
    }

}
void actualizarIdSegmentos(tablaDeSegmentos* tablaDeSegmentos){

    struct tablaDeSegmentos* _tablaDeSegmentos = tablaDeSegmentos->siguiente;

    while(_tablaDeSegmentos != NULL) {
        _tablaDeSegmentos->registro.idSegmento -= 1 ;
        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

}
