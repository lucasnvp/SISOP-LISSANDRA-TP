//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_MEMORIA_H
#define TP_2019_1C_GANK_MID_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/inotify.h>
#include <sys/select.h>

#include <commons/collections/list.h>
#include <commonsfunctions/functions.h>
#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "serializador/estructuras.h"
#include "serializador/handshake.h"

#include "config/Config_memoria.h"
#include "comandos/comandos.h"
#include "estructuras/paginas.h"
#include "estructuras/segmentos.h"
#include "estructuras/marcos.h"
#include "estructuras/gossip.h"
#include "comandosMemoria.h"

char* PATH_CONFIG;
Type_Config* config;
uint32_t configFilePathSize;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logMemoria.txt";
t_log* log_Console;
t_log* log_Memoria;

uint32_t SERVIDOR_FILESYSTEM;
bool MEMORY_READY;

// Variables para el servidor
fd_set master;   	// conjunto maestro de descriptores de fichero

// Variables inotify
uint32_t fd_inotify;
uint32_t wd_inotify;
uint32_t length_inotify;
size_t bufferInotifySize;

// Semaforos
pthread_mutex_t mutexConfig;    // Bloqueo el hilo para cambiar la config

// Variables hilos
pthread_t thread_server;
pthread_t thread_consola;
pthread_t thread_journaling;
pthread_t thread_gossiping;
pthread_t thread_config;

// Dirección de la Memoria Principal
void* memoriaPrincipal;

typedef struct {
    uint32_t cantArgs;
    char * comando;
    char * arg[4];
} t_comandos;

registro_tad* alocar_MemoriaPrincipal();

void inicializarSemaforos();
void inicializarHilos();
void recibir_valores_FileSystem(uint32_t servidorFileSystem);
void init_log(char* pathLog);
void connect_server_FileSystem();
void server(void* args);
void connection_handler(uint32_t socket, uint32_t command);
void memory_console();
void journaling();
void gossiping();
void watching_config();

#endif //TP_2019_1C_GANK_MID_MEMORIA_H