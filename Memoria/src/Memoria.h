//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_MEMORIA_H
#define TP_2019_1C_GANK_MID_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>

#include "config/Config_memoria.h"

#define NUEVA_CONEXION  1
#define INSERT          2

char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/Memoria/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logMemoria.txt";
t_log* log_Console;
t_log* log_Memoria;

uint32_t SERVIDOR_FILESYSTEM;

// Variables para el servidor
fd_set master;   	// conjunto maestro de descriptores de fichero

// Variables hilos
pthread_t thread_server;

void init_log(char* pathLog);
void connect_server_FileSystem();
void server(void* args);
void connection_handler(uint32_t socket, uint32_t command);

#endif //TP_2019_1C_GANK_MID_MEMORIA_H
