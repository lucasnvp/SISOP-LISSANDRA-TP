
// Created by utnso on 09/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

#include "string.h"

#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "serializador/estructuras.h"

#include "../config/Config_memoria.h"
#include "../comandos/comandos.h"
#include "../estructuras/paginas.h"
#include "../estructuras/segmentos.h"
#include "../estructuras/marcos.h"

// Flag para intepretar si el comando es enviado por kernel o por consola
#define CONSOLE_REQUEST -1

t_log* log_Memoria;

void print_console(void (*log_function)(t_log*, const char*), char* message);
char* comando_select(uint32_t SERVIDOR_FILESYSTEM, select_tad* select);
void comando_insert(insert_tad* insert);
void comando_create(create_tad* create, int requestOrigin);
void comando_describe(char* nombreTabla, int requestOrigin);
void comando_drop(char* nombreTabla);
void comando_journal(int requestOrigin);

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
