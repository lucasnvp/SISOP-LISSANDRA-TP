//
// Created by utnso on 06/04/19.
//

#include "Config_kernel.h"

Type_Config load_config(char* path){
    Type_Config config;
    t_config *auxConfig;
    auxConfig = config_create(path);

    config.IP_MEMORIA = strdup(config_get_string_value(auxConfig, "IP_MEMORIA"));
    config.PUERTO_MEMORIA = config_get_int_value(auxConfig, "PUERTO_MEMORIA");
    config.QUANTUM = config_get_int_value(auxConfig, "QUANTUM");
    config.MULTIPROCESAMIENTO = config_get_int_value(auxConfig, "MULTIPROCESAMIENTO");
    config.REFRESH_METADATA = config_get_int_value(auxConfig, "REFRESH_METADATA");
    config.RETARDO = config_get_int_value(auxConfig, "RETARDO");

    config_destroy(auxConfig);

    return config;
}

void print_config(Type_Config auxConfig, t_log* log_Console){
    log_info(log_Console, "----------------------");
    log_info(log_Console, "Configuracion:");
    log_info(log_Console, "IP_MEMORIA = %d",auxConfig.IP_MEMORIA);
    log_info(log_Console, "PUERTO_MEMORIA = %d",auxConfig.PUERTO_MEMORIA);
    log_info(log_Console, "QUANTUM = %d",auxConfig.QUANTUM);
    log_info(log_Console, "MULTIPROCESAMIENTO = %d",auxConfig.MULTIPROCESAMIENTO);
    log_info(log_Console, "REFRESH_METADATA = %d",auxConfig.REFRESH_METADATA);
    log_info(log_Console, "RETARDO = %d",auxConfig.RETARDO);
    log_info(log_Console, "----------------------");
}