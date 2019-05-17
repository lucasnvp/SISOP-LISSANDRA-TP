//
// Created by utnso on 13/05/19.
//

#include "paginas.h"


/*** --------------- PÁGINAS --------------- ***/

// REGISTROS DE PÁGINAS



// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void funcionInsert(char* nombreDeTabla, uint32_t key, char* value) {
    printf("estoy en funcion insert");
    puts(key);
    printf("nombreTabla >> %s \n", nombreDeTabla);
    puts(key);
    printf("value       >> %s \n", value);
    struct tablaDeSegmentos *_TablaDeSegmento;
    _TablaDeSegmento = buscarSegmento(nombreDeTabla);
    puts("buscarSegmento terminada");
    if (_TablaDeSegmento == NULL) {
        _TablaDeSegmento = agregarSegmento(nombreDeTabla);
        printf("\nvolvi de agregar segmento\n");
    }

    uint32_t timestampActual = time(NULL);
    struct tablaDePaginas *_TablaDePaginas;
    _TablaDePaginas = _TablaDeSegmento->registro.tablaDePaginas;
    struct tablaDePaginas* ultimaPagina = NULL;

    while (_TablaDePaginas != NULL) {
        puts("entre al while");
        printf("key punteroAPagina \n \n \n");
        puts(_TablaDePaginas->registro.punteroAPagina->key);
        //busco la pagina por la key
        if (_TablaDePaginas->registro.punteroAPagina->key == key) {
            printf("encontre la key %d", key);
            printf("ts almacenado // fuera if %d", _TablaDePaginas->registro.punteroAPagina->timestamp);
            //comparo timestamps (el actual y el guardado) y actualizo value si corresponde
            if (_TablaDePaginas->registro.punteroAPagina->timestamp > timestampActual) {
                printf("ts actual // dentro if %d", timestampActual);
                printf("ts almacenado // dentro if %d", _TablaDePaginas->registro.punteroAPagina->timestamp);
                printf("actualice la key %d", key);
                _TablaDePaginas->registro.flagModificado = true;
                memcpy(_TablaDePaginas->registro.punteroAPagina,
                        new_registro_tad(timestampActual, key, value),sizeof(registo_tad));
                return;
            }
        }
        ultimaPagina = _TablaDePaginas;
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
    }
    //si no la encuentro la agrego junto a su registro de pagina

    struct tablaDePaginas* nuevoRegistroPagina = malloc(sizeof(tablaDePaginas));

    if(ultimaPagina == NULL){//si es null no tenemos ninguna pagina en el registro
        _TablaDeSegmento->registro.tablaDePaginas = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = (uint32_t) 1;
    } else {
        ultimaPagina->siguienteRegistroPagina = nuevoRegistroPagina;
        nuevoRegistroPagina->registro.numeroPagina = ultimaPagina->registro.numeroPagina + 1;
    }

    nuevoRegistroPagina->registro.punteroAPagina = reservarMarco();
    nuevoRegistroPagina->registro.flagModificado = false;
    nuevoRegistroPagina->registro.ultimoAcceso = timestampActual;

    memcpy(nuevoRegistroPagina->registro.punteroAPagina,new_registro_tad(timestampActual,key,value),
            sizeof(registo_tad));

    printf("valores registro de pagina insertado \n");
    printf("la key es ");
    puts( nuevoRegistroPagina->registro.punteroAPagina->key);
    return;
}


/*** ---------JOURNAL------- ***/

// obtiene el registro más viejo y reenlaza la lista
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

void actualizarIdPaginas(tablaDePaginas* registroMasViejo){
    tablaDePaginas* _TablaDePaginas = registroMasViejo;

    while (_TablaDePaginas->siguienteRegistroPagina != NULL){
        _TablaDePaginas = _TablaDePaginas->siguienteRegistroPagina;
        _TablaDePaginas->registro.numeroPagina -= 1;
    }
    return;
}

