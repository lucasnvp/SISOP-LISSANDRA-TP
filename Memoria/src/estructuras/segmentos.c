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
        if(_TablaDeSegmento->registro.nombreTabla == nombreDeTabla){
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

    if(ultimo==NULL){ //por si aun no fue creado ningún registro de segmento, lo asigno y le coloco su ID
        primerRegistroDeSegmentos = nuevoRegistroSegmento;
        nuevoRegistroSegmento->registro.idSegmento = 1;

    } else { // en caso de que ya haya registros de segmento, lo asigno y le coloco su ID
        ultimo->siguiente = nuevoRegistroSegmento;
        nuevoRegistroSegmento->registro.idSegmento = ultimo->registro.idSegmento + 1;

    }
    nuevoRegistroSegmento->registro.nombreTabla = nombreDeTabla;
    nuevoRegistroSegmento->registro.siguienteRegistroSegmento = NULL;

    return nuevoRegistroSegmento;
}

/*
reg_segmento* obtenerRegistroDeSegmento(char* nombreTabla){
    reg_segmento* _tablaDeSegmentos = tablaDeSegmentos;
    reg_segmento* _ultimoRegistroDeSegmento = NULL;

    while(_tablaDeSegmentos != NULL) {

        _ultimoRegistroDeSegmento = _tablaDeSegmentos;

        // valido si existe la tabla, en caso de existir, la retorno

        if (validarNombreTabla(nombreTabla, _tablaDeSegmentos->nombreTabla)){
            return _tablaDeSegmentos;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguienteRegistroSegmento;
    }

    // en caso de no existir, o no haber registro de segmentos, lo agrego

    return agregarRegistroDeSegmento(nombreTabla, _ultimoRegistroDeSegmento);
}


// Agrega un registro de Segmento. En caso de ser el primero, asigna al puntero tablaDeSegmentos el primer registro creado

reg_segmento* agregarRegistroDeSegmento(char* nombreTabla, reg_segmento* ultimoRegistroDeSegmento){

    //creo el nuevo registro
    reg_segmento* nuevoRegistroDeSegmento = malloc(sizeof(reg_segmento));
    nuevoRegistroDeSegmento->nombreTabla = nombreTabla;
    nuevoRegistroDeSegmento->siguienteRegistroSegmento = NULL;

    // no hay registros de segmento
    if(ultimoRegistroDeSegmento == NULL){
        tablaDeSegmentos = nuevoRegistroDeSegmento;
    } else {
        //hay registros de segmento, lo agrego al final
        ultimoRegistroDeSegmento->siguienteRegistroSegmento = nuevoRegistroDeSegmento;
    }

    return nuevoRegistroDeSegmento;
}


// Inicializa la tabla de segmentos



bool validarNombreTabla(char* nombreBuscado, char* nombreTabla){
    return strcmp(nombreBuscado, nombreTabla) == 0;
}

bool validarExistenciaDeSegmento(char* nombreTabla){
    reg_segmento* _tablaDeSegmentos = tablaDeSegmentos;
    while(_tablaDeSegmentos != NULL) {
        if (validarNombreTabla(nombreTabla, _tablaDeSegmentos->nombreTabla)){
            return true;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguienteRegistroSegmento;
    }
    return false;
}*/