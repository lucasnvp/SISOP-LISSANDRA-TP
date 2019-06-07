//
// Created by utnso on 13/05/19.
//

#include "paginas.h"


/*** --------------- PÁGINAS --------------- ***/

// REGISTROS DE PÁGINAS


// Obtiene el registro más viejo y reenlaza la lista (libera la página)
registro_tad* liberarPagina() {
    tablaDePaginas* registroMasViejo = obtenerRegistroMasViejo();
    return reenlazarRegistros(registroMasViejo);

}

// Obtiene el registro más viejo de todos los segmentos disponibles en la memoria. En caso de no encontrar ninguno, ejecuta journal. //TODO: Journal
tablaDePaginas* obtenerRegistroMasViejo() {
    struct tablaDeSegmentos* _tablaDeSegmentos = primerRegistroDeSegmentos;
    // el registro más viejo, es el primero que encuentro
    tablaDePaginas* registroMasViejo = NULL;
    bool flagJournal = true;

    //busco el primer registro de pagina modificable

    while(flagJournal && _tablaDeSegmentos != NULL){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        while(pagina != NULL){
            if (!pagina->registro.flagModificado){
                flagJournal = false;
                registroMasViejo = pagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

    _tablaDeSegmentos = primerRegistroDeSegmentos;

    // en tanto tenga segmentos para iterar y no tenga que hacer journal (no encontre registro modificable)
    while(!flagJournal && _tablaDeSegmentos != NULL ){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // en tanto tenga páginas dentro de mi segmento
        while ( pagina != NULL){
            // compruebo si el último acceso es menor que el registroMasViejo, y de ser así, compruebo si la página contiene el flag de modificado en false
            if (pagina->registro.ultimoAcceso < registroMasViejo->registro.ultimoAcceso && !pagina->registro.flagModificado){
                registroMasViejo = pagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

    // en este punto no hay ninguna página en ningún segmento que tenga el flag de modificado activado; entonces compruebo el registro más viejo
    if(flagJournal) {
        //ejecutar journal -- recordar llamar a reservarMarco dentro del journal
        log_info(log_Memoria, "La memoria esta FULL. Se procede a realizar un JOURNAL");
        funcionJournal();
    }
    return registroMasViejo;


}

// Reenlaza los registros de páginas
registro_tad* reenlazarRegistros(tablaDePaginas* registroMasViejo) {

    if(registroMasViejo == NULL){
        return reservarMarco();
    }

    struct tablaDeSegmentos* _tablaDeSegmentos = primerRegistroDeSegmentos;

    // en tanto tenga segmentos disponibles
    while(_tablaDeSegmentos != NULL){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // si el registro más viejo es la página del segmento, enlazo en la lista
        if(registroMasViejo == pagina){
            _tablaDeSegmentos->registro.tablaDePaginas = registroMasViejo->siguienteRegistroPagina;
            registro_tad* aux = registroMasViejo->registro.punteroAPagina;
            actualizarIdPaginas(registroMasViejo);
            free(registroMasViejo);
            return aux;
        }
        // en tanto tenga páginas disponibles
        while ( pagina != NULL){
            // si el registro más viejo es la página siguiente, enlazo en la lista
            if(registroMasViejo == pagina->siguienteRegistroPagina){
                pagina->siguienteRegistroPagina = registroMasViejo->siguienteRegistroPagina;
                registro_tad* aux = registroMasViejo->registro.punteroAPagina;
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

