//
// Created by utnso on 09/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

#include "commons/log.h"

#define MEM_SELECT 100

t_log* log_Kernel;

void print_console(void (*log_function)(t_log*, const char*), char* message);
void comando_select(u_int32_t socket);
void comando_insert();
void comando_create();
void comando_describe();
void comando_drop();
void comando_run(char* path);

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
