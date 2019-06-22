//
// Created by utnso on 13/05/19.
//

#ifndef TP_2019_1C_GANK_MID_SEGMENTOS_H
#define TP_2019_1C_GANK_MID_SEGMENTOS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "serializador/estructuras.h"

#include "../config/Config_memoria.h"
#include "../comandos/comandos.h"
#include "paginas.h"
#include "marcos.h"


// Estructura del Registro de un Segmento;
// --- contiene el número de segmento;
// --- su desplazamiento;
// --- y el siguiente segmento

typedef struct reg_segmento{
    uint32_t idSegmento;
    char* nombreTabla;
    struct tablaDePaginas* tablaDePaginas;
}reg_segmento;

// Estructura de la Tabla de Segmentos;
// --- contiene un registro de la tabla de segmentos, y el puntero al siguiente;
// --- cuando el pnutero a siguiente sea null, se habran recorrido todos los segmentos de una submemoria;

typedef struct tablaDeSegmentos {
    reg_segmento registro;
    struct tablaDeSegmentos* siguiente;
}tablaDeSegmentos;

uint32_t tamanoValue;


// Direccion de la tabla de segmentos
struct tablaDeSegmentos* primerRegistroDeSegmentos;

tablaDeSegmentos* buscarSegmento(char* nombreDeTabla);
void reenlazarSegmentos(tablaDeSegmentos* tablaDeSegmentos);
void actualizarIdSegmentos(tablaDeSegmentos* tablaDeSegmentos);
tablaDeSegmentos* agregarSegmento(char* nombreDeTabla);

#endif //TP_2019_1C_GANK_MID_SEGMENTOS_H
