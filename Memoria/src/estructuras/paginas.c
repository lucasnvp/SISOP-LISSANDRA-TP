//
// Created by utnso on 13/05/19.
//

#include "paginas.h"


/*** --------------- PÁGINAS --------------- ***/

// REGISTROS DE PÁGINAS



// Reserva un marco para una Página marcándolo como ocupado y devuelve su dirección de memoria
// Si todos los marcos se encuentran ocupados, se buscará una Página para Liberar

registo_tad* reservarMarco() {
    struct tablaDeMarcos* _tablaDeMarcos = tablaDeMarcos;
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


// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void funcionInsert(char* nombreDeTabla, uint32_t key, char* value) {
    struct tablaDeSegmentos *_TablaDeSegmento;
    _TablaDeSegmento = buscarSegmento(nombreDeTabla);

    if (_TablaDeSegmento == NULL) {
        _TablaDeSegmento = agregarSegmento(nombreDeTabla);
    }

    __uint32_t timestampActual = time(NULL);

    struct tablaDePaginas *_TablaDePaginas;
    _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;


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
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }
    //si no la encuentro la agrego junto a su registro de pagina

}

// Ocupa el marco de página
void ocuparPagina(registo_tad* punteroAPagina, uint32_t timestamp, uint32_t key, char* value ) {
    punteroAPagina = new_registro_tad(timestamp, key, value);
}

/*** ---------JOURNAL------- ***/

// obtiene el registro más viejo y reenlaza la lista
registo_tad* liberarPagina() {

    tablaDePaginas* registroMasViejo = obtenerRegistroMasViejo();
    return reenlazarRegistros(registroMasViejo);

}

// Obtiene el registro más viejo de todos los segmentos disponibles en la memoria. En caso de no encontrar ninguno, ejecuta journal. //TODO: Journal
tablaDePaginas* obtenerRegistroMasViejo() {
    struct tablaDeSegmentos* _tablaDeSegmentos = tablaDeSegmentos;
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


    struct tablaDeSegmentos* _tablaDeSegmentos = tablaDeSegmentos;

    // en tanto tenga segmentos disponibles
    while(_tablaDeSegmentos != NULL){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // si el registro más viejo es la página del segmento, enlazo en la lista
        if(registroMasViejo == pagina){
            *(_tablaDeSegmentos)->registro.tablaDePaginas = *(registroMasViejo)->siguienteRegistroPagina;
            registo_tad* aux = registroMasViejo->registro.punteroAPagina;
            free(registroMasViejo);
            return registroMasViejo->registro.punteroAPagina;
        }
        // en tanto tenga páginas disponibles
        while ( pagina != NULL){
            // si el registro más viejo es la página siguiente, enlazo en la lista
            if(registroMasViejo == pagina->siguienteRegistroPagina){
                *(pagina)->siguienteRegistroPagina = *(registroMasViejo)->siguienteRegistroPagina;
                registo_tad* aux = registroMasViejo->registro.punteroAPagina;
                free(registroMasViejo);
                return registroMasViejo->registro.punteroAPagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }

        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

}

