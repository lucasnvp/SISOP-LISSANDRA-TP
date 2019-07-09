//
// Created by utnso on 13/05/19.
//

#include "paginas.h"


/*** --------------- PÁGINAS --------------- ***/

// REGISTROS DE PÁGINAS


// Obtiene el registro más viejo y reenlaza la lista (libera la página)
registro_tad* liberarPagina(int socket, bool flagModificado) {
    tablaDePaginas* registroMasViejo;
    bool seDebeHacerJournal = verificarPaginas();
    if (socket != CONSOLE_REQUEST && flagModificado) {
            serializar_int(socket, seDebeHacerJournal);
    }

    if (seDebeHacerJournal) {
        if(socket == CONSOLE_REQUEST){
            print_console((void*) log_info, "Memory FULL: Se requiere ejecutar JOURNAL para insertar un nuevo registro");
        }
        registroMasViejo = NULL; // asi activa reservarMarco() en reenlazar registro
        return NULL; // lo hacemos en el caso de que realicemos un insert por consola y tengamos la memoria full
    } else {
        registroMasViejo = obtenerRegistroMasViejo();
    }
    return reenlazarRegistros(socket, registroMasViejo, flagModificado);

}

// Obtiene el registro más viejo de todos los segmentos disponibles en la memoria. En caso de no encontrar ninguno, ejecuta journal. //TODO: Journal
tablaDePaginas* obtenerRegistroMasViejo() {
    struct tablaDeSegmentos* _tablaDeSegmentos = primerRegistroDeSegmentos;
    // el registro más viejo, es el primero que encuentro
    tablaDePaginas* registroMasViejo = NULL;

    //busco el primer registro de pagina modificable

    while(_tablaDeSegmentos != NULL){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        while(pagina != NULL){
            if (!pagina->registro.flagModificado){
                registroMasViejo = pagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

    _tablaDeSegmentos = primerRegistroDeSegmentos;

    // en tanto tenga segmentos para iterar y no tenga que hacer journal (no encontre registro modificable)
    while(_tablaDeSegmentos != NULL ){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // en tanto tenga páginas dentro de mi segmento
        while ( pagina != NULL){
            // compruebo si el último acceso es menor que el registroMasViejo, y de ser así, compruebo si la página contiene el flag de modificado en false
            if ( !pagina->registro.flagModificado && pagina->registro.ultimoAcceso < registroMasViejo->registro.ultimoAcceso){
                registroMasViejo = pagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

    return registroMasViejo;


}

// Reenlaza los registros de páginas
registro_tad* reenlazarRegistros(int socket, tablaDePaginas* registroMasViejo, bool flagModificado) {

    if(registroMasViejo == NULL){
        return reservarMarco(socket, flagModificado);
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

bool verificarPaginas() {
    tablaDeSegmentos* _TablaDeSegmento = primerRegistroDeSegmentos;

    bool seDebeHacerJournal;

    while (_TablaDeSegmento != NULL) {
        tablaDePaginas* _TablaDePagina = _TablaDeSegmento->registro.tablaDePaginas;
        while (_TablaDePagina != NULL) {
            if (!_TablaDePagina->registro.flagModificado) {
                seDebeHacerJournal = false;
                return seDebeHacerJournal;
            }
            _TablaDePagina = _TablaDePagina->siguienteRegistroPagina;
        }

        _TablaDeSegmento = _TablaDeSegmento->siguiente;
    }

    seDebeHacerJournal = true;

    return seDebeHacerJournal;
}

