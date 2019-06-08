//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_FILESYSTEM_H
#define TP_2019_1C_GANK_MID_FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <time.h>

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "serializador/estructuras.h"
#include "serializador/handshake.h"

#include "config/Config_filesystem.h"
#include "comandos/comandos.h"
#include "utils/utils.h"
#include "utils/memtableHanlder.h"


char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/FileSystem/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logFileSystem.txt";
t_log* log_Console;
t_log* log_FileSystem;

// Variables para el servidor
fd_set master;   	// conjunto maestro de descriptores de fichero

// Variables hilos
pthread_t thread_server;
pthread_t thread_consola;

typedef struct {
    uint32_t cantArgs;
    char * comando;
    char * arg[4];
} t_comandos;

void init_log(char* pathLog);
void server(void* args);
void connection_handler(uint32_t socket, uint32_t command);
void consola();

#endif //TP_2019_1C_GANK_MID_FILESYSTEM_H
