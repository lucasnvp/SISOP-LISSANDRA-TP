//
// Created by utnso on 09/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Kernel, message);
    printf("%s", message);
}

void comando_select(){
    print_console((void*) log_info, "Comando select");
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