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
