//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_MEMORIA_H
#define TP_2019_1C_GANK_MID_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <semaphore.h>
#include <pthread.h>


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


pthread_mutex_t mutexJournal; // Semáforo para el Journal -- Se utiliza mutex para garantizar la EM
char* PATH_CONFIG;
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logMemoria.txt";
t_log* log_Console;
t_log* log_Memoria;

uint32_t SERVIDOR_FILESYSTEM;

// Variables para el servidor
fd_set master;   	// conjunto maestro de descriptores de fichero

// Variables hilos
pthread_t thread_server;
pthread_t thread_consola;
pthread_t thread_journaling;
pthread_t thread_gossiping;

// Dirección de la Memoria Principal
void* memoriaPrincipal;

typedef struct {
    uint32_t cantArgs;
    char * comando;
    char * arg[4];
} t_comandos;

//TODO definir estructura de cada registro de la tabla de gossiping(readme)



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

#endif //TP_2019_1C_GANK_MID_MEMORIA_H