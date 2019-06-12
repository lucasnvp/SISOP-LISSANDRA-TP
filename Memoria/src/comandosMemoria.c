
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
                    log_info(log_Memoria, "JOURNAL A FS => TABLA: <%s>\tkey: <%d>\tvalue: <%s>",_TablaDeSegmento->registro.nombreTabla,
                             _tablaDePaginas->registro.punteroAPagina->key,
                             _tablaDePaginas->registro.punteroAPagina->value);
                } else {
                    log_info(log_Memoria, "La tabla %s no existe en File System",_TablaDeSegmento->registro.nombreTabla);
                }
            }
            _tablaDePaginas = _tablaDePaginas->siguienteRegistroPagina;
        }

        char* nombreDeTablaADropear =_TablaDeSegmento->registro.nombreTabla;
        _TablaDeSegmento = _TablaDeSegmento->siguiente;
        funcionDrop(nombreDeTablaADropear);
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
        free(_TablaDeSegmento);
    }

}


char* funcionSelect(uint32_t SERVIDOR_FILESYSTEM, char* nombreDeTabla, uint32_t key){
    struct tablaDeSegmentos* _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    struct tablaDePaginas* _TablaDePaginas = NULL;
    if (_TablaDeSegmento != NULL){
        _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while(_TablaDePaginas != NULL){
            if(_TablaDePaginas->registro.punteroAPagina->key == key){
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

    //solicitar al FS
    // todo usar select_tad como parametro de funcion

    log_info(log_Memoria, "SElECT a FS => TABLA: <%s>\tkey: <%d>\t",
             nombreDeTabla,key);

    select_tad* select = new_select_tad(nombreDeTabla, key);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_SELECT);
    serializar_select(SERVIDOR_FILESYSTEM, select);
    uint32_t confirm = deserializar_int(SERVIDOR_FILESYSTEM);
    if (confirm) {
        serializar_select(SERVIDOR_FILESYSTEM, select);
        char* value = deserializar_string(SERVIDOR_FILESYSTEM);
        funcionInsert(nombreDeTabla, key, value, false); //El flag de modificado en false, por que es exactamento lo que esta en memoria
        return value;
    } else {
        return NULL;
    }
}

// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void funcionInsert(char* nombreDeTabla, uint32_t key, char* value, bool flagModificado) {
    __uint32_t timestampActual = time(NULL);
    struct tablaDeSegmentos *_TablaDeSegmento = buscarSegmento(nombreDeTabla);

    // si la tabla de segmentos es nula, lo agrego y agrego la primera
    if (_TablaDeSegmento == NULL || _TablaDeSegmento->registro.tablaDePaginas == NULL) {
        struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));
        nuevoRegistroPagina->registro.punteroAPagina = reservarMarco();

        _TablaDeSegmento = buscarSegmento(nombreDeTabla);
        if(_TablaDeSegmento == NULL ){
            _TablaDeSegmento = agregarSegmento(nombreDeTabla);
        }
        nuevoRegistroPagina->siguienteRegistroPagina = NULL;
        nuevoRegistroPagina->registro.flagModificado = flagModificado;
        nuevoRegistroPagina->registro.ultimoAcceso = timestampActual;


        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = (uint32_t) 1;

        memcpy(nuevoRegistroPagina->registro.punteroAPagina,
               new_registro_tad(timestampActual, key, value),sizeof(registro_tad));

        return;
    }


    struct tablaDePaginas* _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
    struct tablaDePaginas* ultimaPagina = NULL;

    // en tanto mi tabla de páginas no sea nula
    while (_TablaDePaginas != NULL) {
        //busco la pagina por la key
        if (_TablaDePaginas->registro.punteroAPagina->key == key) {
            //comparo timestamps (el actual y el guardado) y actualizo value si corresponde
            if (_TablaDePaginas->registro.punteroAPagina->timestamp < timestampActual) {
                _TablaDePaginas->registro.flagModificado = true; //actualizo el valor y seteo a true el flag de modificado
                memcpy(_TablaDePaginas->registro.punteroAPagina,
                       new_registro_tad(timestampActual, key, value),sizeof(registro_tad));
                return;
            }
        }
        ultimaPagina = _TablaDePaginas;
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }
    //si no la encuentro la agrego junto a su registro de pagina
    struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));

    nuevoRegistroPagina->registro.punteroAPagina = reservarMarco();
    nuevoRegistroPagina->siguienteRegistroPagina = NULL;
    nuevoRegistroPagina->registro.flagModificado = flagModificado;
    nuevoRegistroPagina->registro.ultimoAcceso = timestampActual;

    _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    if(_TablaDeSegmento == NULL){
        _TablaDeSegmento = agregarSegmento(nombreDeTabla);
        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = (uint32_t) 1;
    }

    ultimaPagina->siguienteRegistroPagina = nuevoRegistroPagina;
    nuevoRegistroPagina->registro.numeroPagina = ultimaPagina->registro.numeroPagina + 1;

    memcpy(nuevoRegistroPagina->registro.punteroAPagina,
           new_registro_tad(timestampActual, key, value),sizeof(registro_tad));
    return;
}