//
// Created by utnso on 09/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Kernel, message);
    printf("%s", message);
}

void comando_select(u_int32_t socket){
    print_console((void*) log_info, "Comando select");
    serializar_int(socket, COMAND_SELECT);
}

void comando_insert(u_int32_t socket){
    print_console((void*) log_info, "Comando insert");
    serializar_int(socket, COMAND_INSERT);
}

void comando_create(u_int32_t socket){
    print_console((void*) log_info, "Comando create");
    serializar_int(socket, COMAND_CREATE);
}

void comando_describe(u_int32_t socket){
    print_console((void*) log_info, "Comando describe");
    serializar_int(socket, COMAND_DESCRIBE);
}

void comando_drop(u_int32_t socket){
    print_console((void*) log_info, "Comando drop");
    serializar_int(socket, COMAND_DROP);
}

void comando_run(char* path){
    if(ValidarArchivo(path)){
        print_console((void*) log_info, "Comando Run");
    } else{
        print_console((void*) log_info, "No se encontro el archivo");
        log_warning(log_Kernel, "No se encontro el archivo");
    }
}