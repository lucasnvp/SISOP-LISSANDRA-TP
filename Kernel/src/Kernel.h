//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_KERNEL_H
#define TP_2019_1C_GANK_MID_KERNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <semaphore.h>

#include <commons/collections/queue.h>
#include <serializador/serializador.h>
#include <parser/parser.h>

#include "config/Config_kernel.h"
#include "comandos/comandos.h"
#include "api/api.h"

pthread_mutex_t mutexMetricas; // Bloqueo el hilo para mostrar las metricas y no se haga otra cosa
sem_t SEM_EXECUTE; //Hay procesos para ejecutar

t_queue * QUEUE_NEW;
t_queue * QUEUE_READY;
t_queue * QUEUE_EXIT;
t_queue * QUEUE_EXEC;

char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/Kernel/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logKernel.txt";
t_log* log_Console;
t_log* log_Kernel;

uint32_t SERVIDOR_MEMORIA;
bool KERNEL_READY;

// Variables hilos
pthread_t thread_consola;
pthread_t thread_metricas;
pthread_t thread_exec;

typedef struct {
    uint32_t cantArgs;
    char * comando;
    char * arg[4];
} t_comandos;

void init_log(char* pathLog);
void connect_server_Memoria();
void consola();
void init_queue_and_sem();
void metricas();
void execute();

#endif //TP_2019_1C_GANK_MID_KERNEL_H
