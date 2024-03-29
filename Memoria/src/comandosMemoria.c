﻿
//
// Created by utnso on 07/06/19.
//

#include "comandosMemoria.h"

void funcionJournal(int requestOrigin) {
    struct tablaDeSegmentos* _TablaDeSegmento = primerRegistroDeSegmentos;
    while(_TablaDeSegmento != NULL){
        struct tablaDePaginas* _tablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while(_tablaDePaginas != NULL){
            if(_tablaDePaginas->registro.flagModificado){
                serializar_int(SERVIDOR_FILESYSTEM, COMAND_INSERT);
                registro_tad* registro = new_registro_tad(_tablaDePaginas->registro.punteroAPagina->timestamp,
                                                    _tablaDePaginas->registro.punteroAPagina->key,
                                                    _tablaDePaginas->registro.punteroAPagina->value);
                serializar_string(SERVIDOR_FILESYSTEM, _TablaDeSegmento->registro.nombreTabla);
                serializar_registro(SERVIDOR_FILESYSTEM, registro);

                free_registro_tad(registro);
    
                uint32_t confirm = deserializar_int(SERVIDOR_FILESYSTEM);
    
                if(confirm){
                    log_info(log_Memoria, "INSERT A FS => TABLA: <%s>\tKEY: <%d>\tVALUE: <%s>",_TablaDeSegmento->registro.nombreTabla,
                             _tablaDePaginas->registro.punteroAPagina->key,
                             _tablaDePaginas->registro.punteroAPagina->value);
                } else {
                    log_info(log_Memoria, "La tabla %s no existe en File System",_TablaDeSegmento->registro.nombreTabla);
                }
            }
            _tablaDePaginas = _tablaDePaginas->siguienteRegistroPagina;
        }

        char* nombreTabla = _TablaDeSegmento->registro.nombreTabla;
        _TablaDeSegmento = _TablaDeSegmento->siguiente;
        funcionDrop(nombreTabla);
    }

    reinicializarRegistros();

    if(requestOrigin != CONSOLE_REQUEST) {
        serializar_int(requestOrigin, true); // envio a kernel que la memoria termino el journal
    }

}

void funcionDrop(char* nombreDeTabla){
    struct tablaDeSegmentos* _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    if(_TablaDeSegmento != NULL){
        //encontro la tabla en memoria

        reenlazarSegmentos(_TablaDeSegmento);
        actualizarIdSegmentos(_TablaDeSegmento);
        log_info(log_Memoria, "DROP EN MEMORIA => TABLA: <%s>\t",
                 _TablaDeSegmento->registro.nombreTabla);

        struct tablaDePaginas* _TablaDePaginas = NULL;
        _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while (_TablaDePaginas != NULL){
            liberarMarco(_TablaDePaginas->registro.punteroAPagina);
            struct tablaDePaginas* paginaALiberar = _TablaDePaginas;
            _TablaDePaginas =_TablaDePaginas->siguienteRegistroPagina;
            free(paginaALiberar);
        }
        free(_TablaDeSegmento);
    }

}


registro_tad* funcionSelect(int socket, select_tad* select){
    struct tablaDeSegmentos* _TablaDeSegmento = buscarSegmento(select->nameTable);
    struct tablaDePaginas* _TablaDePaginas = NULL;
    uint64_t timestampDeAcceso = getCurrentTime();
    if (_TablaDeSegmento != NULL){
        _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while(_TablaDePaginas != NULL){
            if(_TablaDePaginas->registro.punteroAPagina->key == select->key){
                _TablaDePaginas->registro.ultimoAcceso = timestampDeAcceso;
                log_info(log_Memoria, "SELECT EN MEMORIA => TABLA: <%s>\tkey: <%d>\tvalue: <%s>",
                         _TablaDeSegmento->registro.nombreTabla,
                         _TablaDePaginas->registro.punteroAPagina->key,
                         _TablaDePaginas->registro.punteroAPagina->value);
                if ( socket != CONSOLE_REQUEST){
                    serializar_int(socket, false);// mando un memory full en false
                }
                return _TablaDePaginas->registro.punteroAPagina;
            }
            _TablaDePaginas= _TablaDePaginas->siguienteRegistroPagina;
        }
        // En este punto se encuentra la tabla de segmentos pero no la key en sus paginas
    }

    return NULL;
}

registro_tad* solicitarSelectAFileSystem(int socket, select_tad* select) {

    log_info(log_Memoria, "SELECT a FS => TABLA: <%s>\tKEY: <%d>\t",
             select->nameTable,select->key);

    select_tad* select_FS = new_select_tad(select->nameTable, select->key);

    serializar_int(SERVIDOR_FILESYSTEM, COMAND_SELECT);
    serializar_select(SERVIDOR_FILESYSTEM, select_FS);
    free_select_tad(select_FS);

    uint32_t confirm = deserializar_int(SERVIDOR_FILESYSTEM);

    if (confirm) {

        registro_tad* registro = deserializar_registro(SERVIDOR_FILESYSTEM);

        insert_tad* insert = new_insert_tad(select->nameTable, registro->key, registro->value);
//        sem_wait(&semaforoInsert);
        funcionInsert(socket, insert, false, registro->timestamp);
//        sem_post(&semaforoInsert);
        free_insert_tad(insert);
        return registro;
    } else {
        if (socket != CONSOLE_REQUEST) {
            serializar_int(socket, false); // no existe el dato por lo que no hay que hacer journal
        }
        return NULL;
    }
}

// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void funcionInsert(int socket, insert_tad* insert, bool flagModificado, uint64_t timestampDelFS) {

    if(strlen(insert->value) <= tamanoValue){

        if(socket != CONSOLE_REQUEST && flagModificado){
            serializar_int(socket, true); //envio la confirmacion que el insert se puede realizar siendo el tamano valido del value
        }

    } else {
        if(socket != CONSOLE_REQUEST && flagModificado){
            serializar_int(socket, false);  // envio un false porque tamano de value excede el limite
        }
        log_error(log_Memoria, "Value excede el tamanio de value maximo");
        return;
    }

    uint64_t timestamp;
    uint64_t timestampDeAcceso = getCurrentTime();

    if(flagModificado){
        timestamp = timestampDeAcceso;
    } else {
        timestamp = timestampDelFS;
   }

    struct tablaDeSegmentos *_TablaDeSegmento = buscarSegmento(insert->nameTable);

    // si la tabla de segmentos es nula, lo agrego y agrego la primera
    if (_TablaDeSegmento == NULL || _TablaDeSegmento->registro.tablaDePaginas == NULL) {
        struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));
        nuevoRegistroPagina->registro.punteroAPagina = reservarMarco(socket);

        if(nuevoRegistroPagina->registro.punteroAPagina == NULL){
            free(nuevoRegistroPagina);
            log_error(log_Memoria, "Error al ejectuar INSERT: No hay páginas disponibles");
            return;
        }

        _TablaDeSegmento = buscarSegmento(insert->nameTable);
        if(_TablaDeSegmento == NULL ){
            _TablaDeSegmento = agregarSegmento(insert->nameTable);
        }
        nuevoRegistroPagina->siguienteRegistroPagina = NULL;
        nuevoRegistroPagina->registro.flagModificado = flagModificado;
        nuevoRegistroPagina->registro.ultimoAcceso = timestampDeAcceso;


        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = (uint32_t) 1; // porque es el primer registro de la página

        memcpy(nuevoRegistroPagina->registro.punteroAPagina,
               new_registro_tad(timestamp, insert->key, insert->value),sizeof(registro_tad));

        log_info(log_Memoria, "INSERT EN MEMORIA => TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>",
                 _TablaDeSegmento->registro.nombreTabla,
                 nuevoRegistroPagina->registro.punteroAPagina->key,
                 nuevoRegistroPagina->registro.punteroAPagina->value);
        return;
    }


    struct tablaDePaginas* _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
    struct tablaDePaginas* ultimaPagina = NULL;

    // en tanto mi tabla de páginas no sea nula
    while (_TablaDePaginas != NULL) {
        //busco la pagina por la key
        if (_TablaDePaginas->registro.punteroAPagina->key == insert->key) {
            //comparo timestamps (el actual y el guardado) y actualizo value si corresponde
            if (_TablaDePaginas->registro.punteroAPagina->timestamp < timestamp) {
                _TablaDePaginas->registro.flagModificado = true; //actualizo el valor y seteo a true el flag de modificado
                _TablaDePaginas->registro.ultimoAcceso = timestampDeAcceso;
                memcpy(_TablaDePaginas->registro.punteroAPagina,
                       new_registro_tad(timestamp, insert->key, insert->value),sizeof(registro_tad));

                if(socket != CONSOLE_REQUEST){
                    serializar_int(socket, false); // no se debe hacer el journal porq actualizamos el dato
                }
                log_info(log_Memoria, "UPDATE EN MEMORIA => TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>",
                        _TablaDeSegmento->registro.nombreTabla,
                        _TablaDePaginas->registro.punteroAPagina->key,
                        _TablaDePaginas->registro.punteroAPagina->value);
                return;
            }
        }
        ultimaPagina = _TablaDePaginas;
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }
    //si no la encuentro la agrego junto a su registro de pagina
    struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));

    nuevoRegistroPagina->registro.punteroAPagina = reservarMarco(socket);
    if(nuevoRegistroPagina->registro.punteroAPagina == NULL){
        free(nuevoRegistroPagina);
        log_error(log_Memoria, "Error al ejectuar INSERT: No hay páginas disponibles");
        return;
    }
    nuevoRegistroPagina->siguienteRegistroPagina = NULL;
    nuevoRegistroPagina->registro.flagModificado = flagModificado;
    nuevoRegistroPagina->registro.ultimoAcceso = timestampDeAcceso;

    _TablaDeSegmento = buscarSegmento(insert->nameTable);
    if(_TablaDeSegmento == NULL){
        _TablaDeSegmento = agregarSegmento(insert->nameTable);
        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = (uint32_t) 1;
    }

    ultimaPagina->siguienteRegistroPagina = nuevoRegistroPagina;
    nuevoRegistroPagina->registro.numeroPagina = ultimaPagina->registro.numeroPagina + 1;

    memcpy(nuevoRegistroPagina->registro.punteroAPagina,
           new_registro_tad(timestamp, insert->key, insert->value),sizeof(registro_tad));

    log_info(log_Memoria, "INSERT EN MEMORIA => TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>",
             _TablaDeSegmento->registro.nombreTabla,
             nuevoRegistroPagina->registro.punteroAPagina->key,
             nuevoRegistroPagina->registro.punteroAPagina->value);
    return;
}