//
// Created by utnso on 09/04/19.
//

#ifndef TP_2019_1C_GANK_MID_COMANDOS_H
#define TP_2019_1C_GANK_MID_COMANDOS_H

#include "commons/log.h"

#define COMAND_SELECT   100
#define COMAND_INSERT   101
#define COMAND_CREATE   102
#define COMAND_DESCRIBE 103
#define COMAND_DROP     104

t_log* log_Kernel;

void print_console(void (*log_function)(t_log*, const char*), char* message);
void comando_select(u_int32_t socket);
void comando_insert(u_int32_t socket);
void comando_create(u_int32_t socket);
void comando_describe(u_int32_t socket);
void comando_drop(u_int32_t socket);
void comando_run(char* path);

#endif //TP_2019_1C_GANK_MID_COMANDOS_H
