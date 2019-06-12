//
// Created by utnso on 13/05/19.
//

#ifndef TP_2019_1C_GANK_MID_PAGINAS_H
#define TP_2019_1C_GANK_MID_PAGINAS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <time.h>

#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "serializador/estructuras.h"
#include "serializador/handshake.h"

#include "../config/Config_memoria.h"
#include "../comandos/comandos.h"
#include "segmentos.h"
#include "marcos.h"
#include "../comandosMemoria.h"

uint32_t SERVIDOR_FILESYSTEM;

// Estructura de los Registros de la Tabla de Páginas

// ----------> registo_tad es la página <----------

// la página tiene que ser del mismo tamaño que el marco, por lo tanto, tamaño marco = tamaño registro_tad;

// --- contiene el número de página correspondiente; el cual, al ser paginado, es el mismo que la posición del registro dentro de la tabla
// --- la página (registo_tad)
// --- el flag por si se ha modificado el value de la página (registo_tad)

typedef struct reg_tablaDePaginas {
    uint32_t numeroPagina;
    registro_tad* punteroAPagina;
    bool flagModificado;
    uint32_t ultimoAcceso;
}reg_tablaDePaginas;

// Estructura de la Tabla de Páginas dentro de un Segmento en una Sub-Memoria;
// --- contiene un registro de la tabla de páginas, y el puntero al siguiente
// --- cuando el puntero a siguiente sea null, se habrá recorrido toda la tabla de páginas;

typedef struct tablaDePaginas {
    reg_tablaDePaginas registro;
    struct tablaDePaginas* siguienteRegistroPagina;
}tablaDePaginas;

void* memoriaPrincipal;

// Dirección de la Memoria Principal
tablaDePaginas* obtenerRegistroMasViejo();
registro_tad* reenlazarRegistros(tablaDePaginas* registroMasViejo);
registro_tad* liberarPagina();
void actualizarIdPaginas(tablaDePaginas* registroEliminado);
bool verificarPaginas();

#endif //TP_2019_1C_GANK_MID_PAGINAS_H

