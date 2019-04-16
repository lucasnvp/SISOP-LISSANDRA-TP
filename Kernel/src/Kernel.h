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

#include <serializador/serializador.h>

#include "config/Config_kernel.h"
#include "comandos/comandos.h"

char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/Kernel/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logKernel.txt";
t_log* log_Console;
t_log* log_Kernel;

uint32_t SERVIDOR_MEMORIA;

// Variables hilos
pthread_t thread_consola;

typedef struct {
    uint32_t cantArgs;
    char * comando;
    char * arg[4];
} t_comandos;

void init_log(char* pathLog);
void connect_server_Memoria();
void consola();

#endif //TP_2019_1C_GANK_MID_KERNEL_H
