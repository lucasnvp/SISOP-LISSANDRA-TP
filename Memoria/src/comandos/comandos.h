//
// Created by utnso on 09/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

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
#include "../estructuras/paginas.h"
#include "../estructuras/segmentos.h"
#include "../estructuras/marcos.h"

t_log* log_Memoria;

void print_console(void (*log_function)(t_log*, const char*), char* message);
void comando_select();
void comando_insert(char* nombreDeTabla, char* key, char* value);
void comando_create();
void comando_describe();
void comando_drop();

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
