//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_KERNEL_H
#define TP_2019_1C_GANK_MID_KERNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <semaphore.h>
#include <sys/inotify.h>

#include <commons/collections/queue.h>
#include <serializador/serializador.h>
#include <parser/parser.h>

#include "config/Config_kernel.h"
#include "comandos/comandos.h"
#include "api/api.h"

// Semaforos
pthread_mutex_t mutexMetricas;  // Bloqueo el hilo para mostrar las metricas y no se haga otra cosa
pthread_mutex_t mutexConfig;    // Bloqueo el hilo para cambiar la config
sem_t SEM_PLANIFICADOR;         //Hay procesos para planificar

// Variables listas y queue
t_queue * QUEUE_NEW;
t_queue * QUEUE_READY;
t_queue * QUEUE_EXIT;
t_queue * QUEUE_EXEC;

// Variables hilos
pthread_t thread_consola;
pthread_t thread_metricas;
pthread_t thread_exec;
pthread_t thread_config;
pthread_t thread_planificador;

char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/Kernel/src/config/config.txt";
Type_Config* config;
uint32_t configFilePathSize;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logKernel.txt";
t_log* log_Console;
t_log* log_Kernel;

// Variables inotify
uint32_t fd_inotify;
uint32_t wd_inotify;
uint32_t length_inotify;
size_t bufferInotifySize;

uint32_t SERVIDOR_MEMORIA;
bool KERNEL_READY;

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
void watching_config();
void planificador();
bool parser_line(char * line);

#endif //TP_2019_1C_GANK_MID_KERNEL_H
