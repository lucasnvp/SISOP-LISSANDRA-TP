
//
// Created by utnso on 07/06/19.
//

#include "comandosMemoria.h"

void funcionJournal(uint32_t SERVIDOR_FILESYSTEM) {
    struct tablaDeSegmentos* _TablaDeSegmento = primerRegistroDeSegmentos;
    while(_TablaDeSegmento != NULL){
        struct tablaDePaginas* _tablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while(_tablaDePaginas != NULL){
            if(_tablaDePaginas->registro.flagModificado){
                serializar_int(SERVIDOR_FILESYSTEM, COMAND_INSERT);
                insert_tad* insert = new_insert_tad(_TablaDeSegmento->registro.nombreTabla,
                                                    _tablaDePaginas->registro.punteroAPagina->key,
                                                    _tablaDePaginas->registro.punteroAPagina->value);
                serializar_insert(SERVIDOR_FILESYSTEM, insert);
    
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
}

void funcionDrop(char* nombreDeTabla){
    struct tablaDeSegmentos* _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    if(_TablaDeSegmento != NULL){
        //encontro la tabla en memoria
        struct tablaDePaginas* _TablaDePaginas = NULL;
        _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while (_TablaDePaginas != NULL){
            liberarMarco(_TablaDePaginas->registro.punteroAPagina);
            struct tablaDePaginas* paginaALiberar = _TablaDePaginas;
            _TablaDePaginas =_TablaDePaginas->siguienteRegistroPagina;
            free(paginaALiberar);
        }
        reenlazarSegmentos(_TablaDeSegmento);
        actualizarIdSegmentos(_TablaDeSegmento);
        log_info(log_Memoria, "DROP EN MEMORIA => TABLA: <%s>\t",
                 _TablaDeSegmento->registro.nombreTabla);
        free(_TablaDeSegmento);
    }

}


char* funcionSelect(uint32_t SERVIDOR_FILESYSTEM, select_tad* select){
    struct tablaDeSegmentos* _TablaDeSegmento = buscarSegmento(select->nameTable);
    struct tablaDePaginas* _TablaDePaginas = NULL;
    if (_TablaDeSegmento != NULL){
        _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while(_TablaDePaginas != NULL){
            if(_TablaDePaginas->registro.punteroAPagina->key == select->key){
                log_info(log_Memoria, "SELECT EN MEMORIA => TABLA: <%s>\tkey: <%d>\tvalue: <%s>",
                         _TablaDeSegmento->registro.nombreTabla,
                         _TablaDePaginas->registro.punteroAPagina->key,
                         _TablaDePaginas->registro.punteroAPagina->value);
                return _TablaDePaginas->registro.punteroAPagina->value;
            }
            _TablaDePaginas= _TablaDePaginas->siguienteRegistroPagina;
        }
        // En este punto se encuentra la tabla de segmentos pero no la key en sus paginas
    }

    return NULL;
}

char* solicitarSelectAFileSystem(uint32_t socket, select_tad* select) {

    log_info(log_Memoria, "SElECT a FS => TABLA: <%s>\tKEY: <%d>\t",
             select->nameTable,select->key);

    select_tad* select_FS = new_select_tad(select->nameTable, select->key);

    serializar_int(SERVIDOR_FILESYSTEM, COMAND_SELECT);
    serializar_select(SERVIDOR_FILESYSTEM, select_FS);

    uint32_t confirm = deserializar_int(SERVIDOR_FILESYSTEM);

    if (confirm) {
        serializar_select(SERVIDOR_FILESYSTEM, select_FS);
        free_select_tad(select_FS);

        char* value = deserializar_string(SERVIDOR_FILESYSTEM);
        /*
         * uint32_t timestamp = deserializar_int(SERVIDOR_FILESYSTEM);
         * ese timestamp tiene que ser enviado por filesystem al solicitarle el select
         */
        insert_tad* insert = new_insert_tad(select->nameTable, select->key, value);
        funcionInsert(socket, insert, false);
        free_insert_tad(insert);
        return value;
    } else {
        free_select_tad(select_FS);
        return NULL;
    }
}

// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void funcionInsert(uint32_t socket, insert_tad* insert, bool flagModificado) {
    uint32_t timestamp;
    //if(flagModificado){
        timestamp = time(NULL);
    //} else {
   //     timestamp = *el que viene de FS*
   // }

    struct tablaDeSegmentos *_TablaDeSegmento = buscarSegmento(insert->nameTable);

    // si la tabla de segmentos es nula, lo agrego y agrego la primera
    if (_TablaDeSegmento == NULL || _TablaDeSegmento->registro.tablaDePaginas == NULL) {
        struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));
        nuevoRegistroPagina->registro.punteroAPagina = reservarMarco(socket);

        _TablaDeSegmento = buscarSegmento(insert->nameTable);
        if(_TablaDeSegmento == NULL ){
            _TablaDeSegmento = agregarSegmento(insert->nameTable);
        }
        nuevoRegistroPagina->siguienteRegistroPagina = NULL;
        nuevoRegistroPagina->registro.flagModificado = flagModificado;
        nuevoRegistroPagina->registro.ultimoAcceso = timestamp;


        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = (uint32_t) 1;

        memcpy(nuevoRegistroPagina->registro.punteroAPagina,
               new_registro_tad(timestamp, insert->key, insert->value),sizeof(registro_tad));

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
                memcpy(_TablaDePaginas->registro.punteroAPagina,
                       new_registro_tad(timestamp, insert->key, insert->value),sizeof(registro_tad));
                return;
            }
        }
        ultimaPagina = _TablaDePaginas;
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }
    //si no la encuentro la agrego junto a su registro de pagina
    struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));

    nuevoRegistroPagina->registro.punteroAPagina = reservarMarco(socket);
    nuevoRegistroPagina->siguienteRegistroPagina = NULL;
    nuevoRegistroPagina->registro.flagModificado = flagModificado;
    nuevoRegistroPagina->registro.ultimoAcceso = timestamp;

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