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
    serializar_int(socket, MEM_SELECT);
}

void comando_insert(){
    print_console((void*) log_info, "Comando insert");
}

void comando_create(){
    print_console((void*) log_info, "Comando create");
}

void comando_describe(){
    print_console((void*) log_info, "Comando describe");
}

void comando_drop(){
    print_console((void*) log_info, "Comando drop");
}

void comando_run(char* path){
    if(ValidarArchivo(path)){
        print_console((void*) log_info, "Comando Run");
    } else{
        print_console((void*) log_info, "No se encontro el archivo");
        log_warning(log_Kernel, "No se encontro el archivo");
    }
}