//
// Created by utnso on 09/04/19.
//

#include "comandos.h"


void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Memoria, message);
    printf("%s", message);
}

void comando_select(char* nombreDeTabla, char* key){
    print_console((void*) log_info, "Comando select");
    funcionSelect(nombreDeTabla, atoi(key));
}

void comando_insert(char* nombreDeTabla, char* key, char* value){
    if(strlen(value) > tamanoValue) {
        print_console((void*) log_error, "El value excede el tamaño máximo.");
    } else {
        print_console((void *) log_info, "Comando insert");
        //funcionInsert(nombreDeTabla, atoi(key), value);
        for (uint32_t index = 0; index < atoi(key); index++) { //este for es pa testeo
            funcionInsert(nombreDeTabla, index, value);
            sleep(1);
        }
    }
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

