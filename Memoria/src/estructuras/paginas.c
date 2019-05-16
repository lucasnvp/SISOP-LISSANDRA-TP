//
// Created by utnso on 13/05/19.
//

#include "paginas.h"


/*** --------------- PÁGINAS --------------- ***/

// REGISTROS DE PÁGINAS



// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void funcionInsert(char* nombreDeTabla, uint32_t key, char* value) {
    struct tablaDeSegmentos *_TablaDeSegmento;
    _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    // si la tabla de segmentos es nula, lo agrego
    if (_TablaDeSegmento == NULL) {
        _TablaDeSegmento = agregarSegmento(nombreDeTabla);
    }

    __uint32_t timestampActual = time(NULL);

    struct tablaDePaginas *_TablaDePaginas;
    _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;

    struct tablaDePaginas* ultimaPagina = NULL;

    // en tanto mi tabla de páginas no sea nula
    while (_TablaDePaginas != NULL) {
        //busco la pagina por la key
        if (_TablaDePaginas->registro.punteroAPagina->key == key) {
            //comparo timestamps (el actual y el guardado) y actualizo value si corresponde
            if (_TablaDePaginas->registro.punteroAPagina->timestamp > timestampActual) {
                _TablaDePaginas->registro.flagModificado = true;
                memcpy(_TablaDePaginas->registro.punteroAPagina,
                        new_registro_tad(timestampActual, key, value),sizeof(registo_tad));
            }
        }
        ultimaPagina = _TablaDePaginas;
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }
    //si no la encuentro la agrego junto a su registro de pagina

    struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));

    if(ultimaPagina == NULL){//si es null no tenemos ninguna pagina en el registro
        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = 1;
    } else {
        ultimaPagina->siguienteRegistroPagina = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = ultimaPagina->registro.numeroPagina + 1;
    }

    nuevoRegistroPagina->registro.punteroAPagina = reservarMarco();
    nuevoRegistroPagina->registro.flagModificado = false;
    nuevoRegistroPagina->registro.ultimoAcceso = timestampActual;

    memcpy(nuevoRegistroPagina->registro.punteroAPagina,new_registro_tad(timestampActual,key,value),
            sizeof(registo_tad));
    return;
}

// Obtiene el registro más viejo y reenlaza la lista (libera la página)
registo_tad* liberarPagina() {
    tablaDePaginas* registroMasViejo = obtenerRegistroMasViejo();
    return reenlazarRegistros(registroMasViejo);

}

// Obtiene el registro más viejo de todos los segmentos disponibles en la memoria. En caso de no encontrar ninguno, ejecuta journal. //TODO: Journal
tablaDePaginas* obtenerRegistroMasViejo() {
    struct tablaDeSegmentos* _tablaDeSegmentos = primerRegistroDeSegmentos;
    // el registro más viejo, es el primero que encuentro
    tablaDePaginas* registroMasViejo = _tablaDeSegmentos->registro.tablaDePaginas;
    // en tanto tenga segmentos para iterar
    while( _tablaDeSegmentos != NULL ){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // en tanto tenga páginas dentro de mi segmento
        while ( pagina != NULL){
            // compruebo si el último acceso es menor que el registroMasViejo, y de ser así, compruebo si la página contiene el flag de modificado en false
            if (pagina->registro.ultimoAcceso < registroMasViejo->registro.ultimoAcceso & !pagina->registro.flagModificado){
                registroMasViejo = pagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

    // en este punto no hay ninguna página en ningún segmento que tenga el flag de modificado activado; entonces compruebo el registro más viejo
    if(registroMasViejo == _tablaDeSegmentos->registro.tablaDePaginas & registroMasViejo->registro.flagModificado) {
        //ejecutar journal
    }
    return registroMasViejo;
}

// Reenlaza los registros de páginas
registo_tad* reenlazarRegistros(tablaDePaginas* registroMasViejo) {

    struct tablaDeSegmentos* _tablaDeSegmentos = primerRegistroDeSegmentos;

    // en tanto tenga segmentos disponibles
    while(_tablaDeSegmentos != NULL){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // si el registro más viejo es la página del segmento, enlazo en la lista
        if(registroMasViejo == pagina){
            *(_tablaDeSegmentos)->registro.tablaDePaginas = *(registroMasViejo)->siguienteRegistroPagina;
            registo_tad* aux = registroMasViejo->registro.punteroAPagina;
            actualizarIdPaginas(registroMasViejo);
            free(registroMasViejo);
            return aux;
        }
        // en tanto tenga páginas disponibles
        while ( pagina != NULL){
            // si el registro más viejo es la página siguiente, enlazo en la lista
            if(registroMasViejo == pagina->siguienteRegistroPagina){
                *(pagina)->siguienteRegistroPagina = *(registroMasViejo)->siguienteRegistroPagina;
                registo_tad* aux = registroMasViejo->registro.punteroAPagina;
                actualizarIdPaginas(registroMasViejo);
                free(registroMasViejo);
                return aux;
            }
            pagina = pagina->siguienteRegistroPagina;
        }

        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

}

// Actualiza los ID de las páginas ante una liberación de un registro de página
// --- a partir del registro más viejo encontrado, el cual será removido
void actualizarIdPaginas(tablaDePaginas* registroMasViejo){
    tablaDePaginas* _TablaDePaginas = registroMasViejo;
    // itero en tanto tenga un registro pŕoximo
    // decremento el número de página en 1
    while (_TablaDePaginas->siguienteRegistroPagina != NULL){
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
        _TablaDePaginas->registro.numeroPagina -= 1;
    }
    return;
}

