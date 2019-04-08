//
// Created by utnso on 06/04/19.
//

#ifndef TP_2019_1C_GANK_MID_KERNEL_H
#define TP_2019_1C_GANK_MID_KERNEL_H

#include <stdio.h>
#include <stdlib.h>

#include "config/Config_kernel.h"

char* PATH_CONFIG = "/home/utnso/Gank-mid/tp-2019-1c-Gank-mid/Kernel/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Gank-mid/Logs/logKernel.txt";
t_log* log_Console;
t_log* log_Kernel;

uint32_t SERVIDOR_MEMORIA;

void init_log(char* pathLog);
void connect_server_Memoria();

#endif //TP_2019_1C_GANK_MID_KERNEL_H
