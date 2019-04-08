//
// Created by utnso on 06/04/19.
//

#include "Kernel.h"

int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Kernel");

    //Inicializar Log
    init_log(PATH_LOG);

    //Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);

    //Conexion al servidor FileSystem
    connect_server_Memoria();

    while(true){

    }

    return EXIT_SUCCESS;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "Kernel", true, LOG_LEVEL_INFO);
    log_Kernel = log_create(pathLog, "Kernel", false, LOG_LEVEL_INFO);
}

void connect_server_Memoria(){
    //Conexion al servidor Memoria
    SERVIDOR_MEMORIA = connect_server(config.IP_MEMORIA,config.PUERTO_MEMORIA);

    //Si conecto, informo
    if(SERVIDOR_MEMORIA > 1){
        log_info(log_Console,"Connected successfully to the Memory");
    } else{
        log_warning(log_Console, "No se puedo conectar al servidor de Memoria");
        exit(EXIT_SUCCESS);
    }
}