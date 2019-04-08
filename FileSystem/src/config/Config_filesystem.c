//
// Created by utnso on 07/04/19.
//

#include "Config_filesystem.h"

Type_Config load_config(char* path){
    Type_Config config;
    t_config *auxConfig;
    auxConfig = config_create(path);

    config.PUERTO_ESCUCHA = config_get_int_value(auxConfig, "PUERTO_ESCUCHA");
    config.CANT_CONEXIONES = config_get_int_value(auxConfig, "CANT_CONEXIONES");
    config.PUNTO_MONTAJE = strdup(config_get_string_value(auxConfig, "PUNTO_MONTAJE"));
    config.RETARDO = config_get_int_value(auxConfig, "RETARDO");
    config.TAMANO_VALUE = config_get_int_value(auxConfig, "TAMANO_VALUE");
    config.TIEMPO_DUMP = config_get_int_value(auxConfig, "TIEMPO_DUMP");

    config_destroy(auxConfig);

    return config;
}

void print_config(Type_Config auxConfig, t_log* log_Console){
    log_info(log_Console, "----------------------");
    log_info(log_Console, "Configuracion:");
    log_info(log_Console, "PUERTO_ESCUCHA = %d",auxConfig.PUERTO_ESCUCHA);
    log_info(log_Console, "CANT_CONEXIONES = %d",auxConfig.CANT_CONEXIONES);
    log_info(log_Console, "PUNTO_MONTAJE = %s",auxConfig.PUNTO_MONTAJE);
    log_info(log_Console, "RETARDO = %d",auxConfig.RETARDO);
    log_info(log_Console, "TAMANO_VALUE = %d",auxConfig.TAMANO_VALUE);
    log_info(log_Console, "TIEMPO_DUMP = %d",auxConfig.TIEMPO_DUMP);
    log_info(log_Console, "----------------------");
}