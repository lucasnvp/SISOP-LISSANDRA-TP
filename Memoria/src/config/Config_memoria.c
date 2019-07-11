//
// Created by utnso on 07/04/19.
//

#include "Config_memoria.h"

Type_Config* load_config(char* path){
    Type_Config* config = malloc(sizeof(Type_Config));
    t_config *auxConfig;
    auxConfig = config_create(path);

    config->PUERTO_ESCUCHA = config_get_int_value(auxConfig, "PUERTO_ESCUCHA");
    config->CANT_CONEXIONES = config_get_int_value(auxConfig, "CANT_CONEXIONES");
    config->IP_FS = strdup(config_get_string_value(auxConfig, "IP_FS"));
    config->PUERTO_FS = config_get_int_value(auxConfig, "PUERTO_FS");
    config->IP_SEEDS = config_get_array_value(auxConfig, "IP_SEEDS");
    config->PUERTO_SEEDS = config_get_array_value(auxConfig, "PUERTO_SEEDS");
    config->RETARDO_MEM = config_get_int_value(auxConfig, "RETARDO_MEM");
    config->RETARDO_FS = config_get_int_value(auxConfig, "RETARDO_FS");
    config->TAM_MEM = config_get_int_value(auxConfig, "TAM_MEM");
    config->RETARDO_JOURNAL = config_get_int_value(auxConfig, "RETARDO_JOURNAL");
    config->RETARDO_GOSSIPING = config_get_int_value(auxConfig, "RETARDO_GOSSIPING");
    config->MEMORY_NUMBER = config_get_int_value(auxConfig, "MEMORY_NUMBER");

    config_destroy(auxConfig);

    return config;
}

void print_config(Type_Config* auxConfig, t_log* log_Console){
    log_info(log_Console, "----------------------");
    log_info(log_Console, "Configuracion:");
    log_info(log_Console, "PUERTO_ESCUCHA = %d",auxConfig->PUERTO_ESCUCHA);
    log_info(log_Console, "CANT_CONEXIONES = %d",auxConfig->CANT_CONEXIONES);
    log_info(log_Console, "IP_FS = %s",auxConfig->IP_FS);
    log_info(log_Console, "PUERTO_FS = %d",auxConfig->PUERTO_FS);

    log_info(log_Console, "SEEDS : IP | Port:");
    for(i=0 ; auxConfig->IP_SEEDS[i] != NULL ; i++){
        log_info(log_Console, "[%d]: %s %s",i, auxConfig->IP_SEEDS[i], auxConfig->PUERTO_SEEDS[i]);
    }

    log_info(log_Console, "RETARDO_MEM = %d",auxConfig->RETARDO_MEM);
    log_info(log_Console, "RETARDO_FS = %d",auxConfig->RETARDO_FS);
    log_info(log_Console, "TAM_MEM = %d",auxConfig->TAM_MEM);
    log_info(log_Console, "RETARDO_JOURNAL = %d",auxConfig->RETARDO_JOURNAL);
    log_info(log_Console, "RETARDO_GOSSIPING = %d",auxConfig->RETARDO_GOSSIPING);
    log_info(log_Console, "MEMORY_NUMBER = %d",auxConfig->MEMORY_NUMBER);
    log_info(log_Console, "----------------------");
}

void free_config(Type_Config* config){
    free(config->IP_FS);
    free(config->IP_SEEDS);
    free(config->PUERTO_SEEDS);
    free(config);
}