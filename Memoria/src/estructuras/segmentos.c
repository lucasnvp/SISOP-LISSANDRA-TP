//
// Created by utnso on 13/05/19.
//

#include "segmentos.h"

//busca en los segmentos, si lo encuentra lo retorna,
// si no lo encuentra lo crea

tablaDeSegmentos* buscarSegmento(char* nombreDeTabla){
    printf("estoy dentro de buscar segmento \n");
    struct tablaDeSegmentos* _TablaDeSegmento;
    _TablaDeSegmento = primerRegistroDeSegmentos;

    while(_TablaDeSegmento != NULL){
        printf("tabla de segmento NO es null \n");
        if(_TablaDeSegmento->registro.nombreTabla == nombreDeTabla){
            printf("encontre la tabla \n");
            return _TablaDeSegmento;
        }
        _TablaDeSegmento = _TablaDeSegmento->siguiente;
    }
    printf("no encontre la tabla \n");
    return NULL;
}

tablaDeSegmentos* agregarSegmento(char* nombreDeTabla){
    printf("estoy dentro de agregar segmento \n");
    struct tablaDeSegmentos* _TablaDeSegmento;
    _TablaDeSegmento = primerRegistroDeSegmentos;
    struct tablaDeSegmentos* ultimo = NULL;

    struct tablaDeSegmentos* nuevoRegistroSegmento = malloc(sizeof(tablaDeSegmentos));

    while(_TablaDeSegmento != NULL){
        printf("busco el ultimo segmento \n");
        ultimo = _TablaDeSegmento;
        _TablaDeSegmento = _TablaDeSegmento->siguiente;
    }

    if(ultimo==NULL){ //por si aun no fue creado ningur registro de segmento
        printf("no hay registro de segmento \n");
        primerRegistroDeSegmentos = nuevoRegistroSegmento;
        nuevoRegistroSegmento->registro.idSegmento = (uint32_t) 1;
        printf("agregue el registro \n");
        printf("%d", nuevoRegistroSegmento->registro.idSegmento);
    } else {
        printf("hay un ultimo registro de segmento \n");
        ultimo->siguiente = nuevoRegistroSegmento;
        nuevoRegistroSegmento->registro.idSegmento = ultimo->registro.idSegmento + 1;
        printf("agregue el registro \n");
        puts(nuevoRegistroSegmento->registro.idSegmento);

    }
    nuevoRegistroSegmento->registro.nombreTabla = nombreDeTabla;
    nuevoRegistroSegmento->registro.tablaDePaginas = NULL;
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