//
// Created by utnso on 06/04/19.
//

#include "Memoria.h"

int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Memoria");

    //Inicializar Log
    init_log(PATH_LOG);

    //Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);

    return EXIT_SUCCESS;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "Memoria", true, LOG_LEVEL_INFO);
    log_Memoria = log_create(pathLog, "Memoria", false, LOG_LEVEL_INFO);
}