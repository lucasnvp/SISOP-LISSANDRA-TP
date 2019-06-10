//
// Created by utnso on 19/04/19.
//

#ifndef TP_2019_1C_GANK_MID_API_H
#define TP_2019_1C_GANK_MID_API_H

#include <semaphore.h>

#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <serializador/serializador.h>
#include <serializador/estructuras.h>
#include <serializador/handshake.h>

#include "../metadata/metadata.h"
#include "../criterios/criterios.h"

t_log* log_Kernel_api;

void api_select(char* tabla, u_int16_t key);
void api_insert(char* tabla, u_int16_t key, char* value);
void api_create(char* tabla, char* consistencia, u_int32_t particiones, u_int32_t compactacion);
void api_describe(char* tabla);
void api_describe_all();
void api_drop(u_int32_t socket, char* tabla);

#endif //TP_2019_1C_GANK_MID_API_H
