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

#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "serializador/estructuras.h"

#include "config/Config_memoria.h"
#include "comandos/comandos.h"

#define NUEVA_CONEXION  1
#define COMAND_SELECT   100
#define COMAND_INSERT   101
#define COMAND_CREATE   102
#define COMAND_DESCRIBE 103
#define COMAND_DROP     104

char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/Memoria/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logMemoria.txt";
t_log* log_Console;
t_log* log_Memoria;

uint32_t SERVIDOR_FILESYSTEM;


// Estructura de los registros de la tabla de gossiping

typedef struct reg_gossiping{
    uint32_t idMemoria;
    char** ipSeeds;
    char** puertoSeeds;

}reg_gossiping;

// Estructura de los registros de la tabla de páginas

typedef struct reg_pagina{
    uint32_t idPagina;
    registo_tad* punteroAMarco;
    bool flagModificado;
    reg_pagina* siguienteRegistroPagina;
}reg_pagina;

//Estructura de los registro de la tabla de segmentacion

typedef struct reg_segmento{
    uint32_t idSegmento;
    char* nombreTabla;
    uint32_t cantPaginas;
    char* consistency;
    reg_pagina* primerRegistroPagina;
    reg_segmento* siguienteRegistroSegmento;
}reg_segmento;

uint32_t tamanoValue;
uint32_t tiempoDump;

typedef reg_marco{
    uint32_t numeroMarco;
    bool marcoOcupado;
}reg_marco;

// Direccion de la tabla de segmentos
reg_segmento* tablaDeSegmentos;

//Direccion de la tabla de marcos
reg_marco* tablaDeMarcos;

// Variables para el servidor
fd_set master;   	// conjunto maestro de descriptores de fichero

// Variables hilos
pthread_t thread_server;
pthread_t thread_consola;

// Dirección de la Memoria Principal
void* memoriaPrincipal;

typedef struct {
    uint32_t cantArgs;
    char * comando;
    char * arg[4];
} t_comandos;

//TODO definir estructura de cada registro de la tabla de gossiping(readme)


bool validarNombreTabla(char* nombreBuscado, char* nombreTabla);
bool validarExistenciaDeSegmento(char* nombreTabla);

reg_segmento* obtenerRegistroDeSegmento(char* nombreTabla);
reg_segmento* agregarRegistroDeSegmento(char* nombreTabla, reg_segmento* ultimoSegmento);

registo_tad* alocar_MemoriaPrincipal();

void crearRegistroDeSegmento(char* nombreTabla);
void recibir_valores_FileSystem(uint32_t servidorFileSystem);
void desalocar_MemoriaPrincipal();
void init_log(char* pathLog);
void connect_server_FileSystem();
void server(void* args);
void connection_handler(uint32_t socket, uint32_t command);
void memory_console();

#endif //TP_2019_1C_GANK_MID_MEMORIA_H
