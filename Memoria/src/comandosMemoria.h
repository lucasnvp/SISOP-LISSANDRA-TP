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

registro_tad* funcionSelect(select_tad* select);
registro_tad* solicitarSelectAFileSystem(int socket, select_tad* select);

void funcionInsert(int requestOrigin, insert_tad* insert, bool flagModificado, uint64_t timestampDelFS);
void funcionDrop(char* nombreDeTabla);
void funcionJournal(int requestOrigin);

#endif //TP_2019_1C_GANK_MID_COMANDOSMEMORIA_H
