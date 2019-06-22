//
//
// Created by utnso on 07/06/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOSMEMORIA_H
#define TP_2019_1C_GANK_MID_COMANDOSMEMORIA_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <time.h>
#include <semaphore.h>

#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "serializador/estructuras.h"
#include "serializador/handshake.h"

#include "./config/Config_memoria.h"
#include "./comandos/comandos.h"
#include "estructuras/segmentos.h"
#include "estructuras/marcos.h"
#include "estructuras/paginas.h"

sem_t semaforoDrop;
sem_t semaforoInsert;

char* funcionSelect(select_tad* select);
char* solicitarSelectAFileSystem(int socket, select_tad* select);
void funcionInsert(int socket, insert_tad* insert, bool flagModificado);
void funcionDrop(char* nombreDeTabla);
void funcionJournal(uint32_t SERVIDOR_FILESYSTEM);

#endif //TP_2019_1C_GANK_MID_COMANDOSMEMORIA_H
