//
// Created by utnso on 16/05/19.
//

#ifndef TP_2019_1C_GANK_MID_MARCOS_H
#define TP_2019_1C_GANK_MID_MARCOS_H

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
#include "segmentos.h"

// Estructura de la Tabla de Marcos;
// --- contiene el número de marco, el cual es el mismo que el número de página dentro de la tabla;
// --- el flag de marco ocupado
// --- el puntero al siguiente marco

typedef struct reg_marco{
    uint32_t numeroMarco;
    bool marcoOcupado;
}reg_marco;

typedef struct tablaDeMarcos {
    reg_marco registro;
    struct tablaDeMarcos *siguiente;
}tablaDeMarcos;

//Direccion de la tabla de marcos
struct tablaDeMarcos* primerRegistroDeMarcos;
__uint32_t cantidadDeMarcos;


registro_tad* reservarMarco(); // reserva un marco
void inicializarMarcos(__uint32_t tamanioMemoria);

#endif //TP_2019_1C_GANK_MID_MARCOS_H
