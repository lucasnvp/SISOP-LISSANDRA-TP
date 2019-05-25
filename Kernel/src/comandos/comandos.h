//
// Created by utnso on 09/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

#include <semaphore.h>

#include <commons/log.h>
#include <commons/collections/queue.h>
#include <commonsfunctions/functions.h>

#include "../api/api.h"
#include "../tads/script.h"

#define COMAND_SELECT   100
#define COMAND_INSERT   101
#define COMAND_CREATE   102
#define COMAND_DESCRIBE 103
#define COMAND_DROP     104

t_log* log_Kernel;

void print_console(void (*log_function)(t_log*, const char*), char* message);
void comando_select(u_int32_t socket, char* tabla, u_int16_t key);
void comando_insert(u_int32_t socket, char* tabla, u_int16_t key, char* value);
void comando_create(u_int32_t socket, char* tabla, char* consistencia, u_int32_t particiones, u_int32_t compactacion);
void comando_describe(u_int32_t socket, char* tabla);
void comando_drop(u_int32_t socket, char* tabla);
void comando_run(char* path, t_queue* QUEUE_READY, sem_t* SEM_PLANIFICADOR);
void comando_metrics();

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
