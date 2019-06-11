//
// Created by utnso on 07/06/19.
//

#include "comandosMemoria.h"

void funcionJournal(uint32_t SERVIDOR_FILESYSTEM) {
    struct tablaDeSegmentos* _TablaDeSegmento = primerRegistroDeSegmentos;
    while(_TablaDeSegmento != NULL){
        struct tablaDePaginas* _tablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while(_tablaDePaginas != NULL){
                        serializar_int(SERVIDOR_FILESYSTEM, COMAND_INSERT);
                        insert_tad* insert = new_insert_tad(_TablaDeSegmento->registro.nombreTabla,
                                                            _tablaDePaginas->registro.punteroAPagina->key,
                                                            _tablaDePaginas->registro.punteroAPagina->value);
                        serializar_insert(SERVIDOR_FILESYSTEM, insert);

                        uint32_t confirm = deserializar_int(SERVIDOR_FILESYSTEM);

                        if(confirm){
                            log_info(log_Memoria, "INSERT => TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>",_TablaDeSegmento->registro.nombreTabla,
                                     _tablaDePaginas->registro.punteroAPagina->key,
                                     _tablaDePaginas->registro.punteroAPagina->value);
                        } else {
                            log_info(log_Memoria, "La tabla %s no existe en File System",_TablaDeSegmento->registro.nombreTabla);
                        }
            _tablaDePaginas = _tablaDePaginas->siguienteRegistroPagina;
        }

        tablaDeSegmentos* tablaDeSegmentosAux = _TablaDeSegmento;

        funcionDrop(tablaDeSegmentosAux->registro.nombreTabla);

        _TablaDeSegmento = _TablaDeSegmento->siguiente;
    }

}

void funcionDrop(char* nombreDeTabla){
    string_to_upper(nombreDeTabla);
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


char* funcionSelect(uint32_t SERVIDOR_FILESYSTEM, char* nombreDeTabla, uint32_t key){
    string_to_upper(nombreDeTabla);
    struct tablaDeSegmentos* _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    struct tablaDePaginas* _TablaDePaginas = NULL;
    if (_TablaDeSegmento != NULL){
        _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
        while(_TablaDePaginas != NULL){
            if(_TablaDePaginas->registro.punteroAPagina->key == key){
                log_info(log_Memoria, "SELECT => TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>",
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
    select_tad* select = new_select_tad(nombreDeTabla, key);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_SELECT);
    serializar_select(SERVIDOR_FILESYSTEM, select);
    uint32_t confirm = deserializar_int(SERVIDOR_FILESYSTEM);
    if (confirm) {
        serializar_select(SERVIDOR_FILESYSTEM, select);
        char* value = deserializar_string(SERVIDOR_FILESYSTEM);
        funcionInsert(nombreDeTabla, key, value);
        return value;
    } else {
        return NULL;
    }
}

// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void funcionInsert(char* nombreDeTabla, uint32_t key, char* value) {
    string_to_upper(nombreDeTabla);
    struct tablaDeSegmentos *_TablaDeSegmento = NULL;
    _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    // si la tabla de segmentos es nula, lo agrego
    if (_TablaDeSegmento == NULL) {
        _TablaDeSegmento = agregarSegmento(nombreDeTabla);
    }

    __uint32_t timestampActual = time(NULL);

    struct tablaDePaginas *_TablaDePaginas;
    _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;


    // en tanto mi tabla de páginas no sea nula
    while (_TablaDePaginas != NULL) {
        //busco la pagina por la key
        if (_TablaDePaginas->registro.punteroAPagina->key == key) {
            //comparo timestamps (el actual y el guardado) y actualizo value si corresponde
            if (_TablaDePaginas->registro.punteroAPagina->timestamp < timestampActual) {
                _TablaDePaginas->registro.flagModificado = true;
                memcpy(_TablaDePaginas->registro.punteroAPagina,
                       new_registro_tad(timestampActual, key, value),sizeof(registro_tad));
                return;
            }
        }
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }
    //si no la encuentro la agrego junto a su registro de pagina
    struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));

    nuevoRegistroPagina->registro.punteroAPagina = reservarMarco();
    nuevoRegistroPagina->siguienteRegistroPagina = NULL;
    nuevoRegistroPagina->registro.flagModificado = false;
    nuevoRegistroPagina->registro.ultimoAcceso = timestampActual;

    struct tablaDePaginas* ultimaPagina = NULL;
    _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
    while(_TablaDePaginas != NULL){
        ultimaPagina = _TablaDePaginas;
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }

    if(ultimaPagina == NULL){//si es null no tenemos ninguna pagina en el registro
        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = (uint32_t) 1;
    } else {
        ultimaPagina->siguienteRegistroPagina = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = ultimaPagina->registro.numeroPagina + 1;
    }

    memcpy(nuevoRegistroPagina->registro.punteroAPagina,
           new_registro_tad(timestampActual, key, value),sizeof(registro_tad));

    log_info(log_Memoria, "INSERT EN MEMORIA => TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>",
             _TablaDeSegmento->registro.nombreTabla,
             nuevoRegistroPagina->registro.punteroAPagina->key,
             nuevoRegistroPagina->registro.punteroAPagina->value);
    return;
}

void funcionCreate(create_tad* create) {

    serializar_int(SERVIDOR_FILESYSTEM, COMAND_CREATE);
    serializar_create(SERVIDOR_FILESYSTEM, create);
    bool confirmacionExistencia = deserializar_int(SERVIDOR_FILESYSTEM);

    if(confirmacionExistencia) {
        log_info(log_Memoria,
                 "Ya existe la TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                 create->nameTable, create->consistencia, create->particiones, create->compactacion);
    } else {

        agregarSegmento(create->nameTable);

        log_info(log_Memoria,
                 "CREATE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                 create->nameTable, create->consistencia, create->particiones, create->compactacion);

    }
    free_create_tad(create);
}

void funcionDescribe(char* nombreTabla) {
    log_info(log_Memoria, "DESCRIBE => TABLA: <%s>\t", nombreTabla);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_DESCRIBE);
    serializar_string(SERVIDOR_FILESYSTEM, nombreTabla);
    free(nombreTabla);
}

void funcionDescribeAll(bool origin) {
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_DESCRIBE_ALL);

    t_list* listDummy = deserializar_describe_all(SERVIDOR_FILESYSTEM);

    void print_element_stack(void* element){
        describe_tad* describe = element;
        log_info(log_Memoria,
                 "DESCRIBE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                 describe->nameTable, describe->consistencia, describe->particiones, describe->compactacion);
    }

    list_iterate(listDummy, print_element_stack);
    if (origin) {
    log_info(log_Memoria, "Se recibio del FS el describe all, se envia al Kernel");
    serializar_describe_all(socket, listDummy);
    list_destroy(listDummy);
    }
}