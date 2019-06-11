//
// Created by utnso on 09/04/19.
//

#include "comandos.h"


void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Memoria, message);
    printf("%s", message);
}

void comando_select(uint32_t SERVIDOR_FILESYSTEM, char* nombreDeTabla, uint32_t key){
    print_console((void*) log_info, "Comando select");
    funcionSelect(SERVIDOR_FILESYSTEM, nombreDeTabla, key);
}

void comando_insert(char* nombreDeTabla, char* key, char* value){
    print_console((void*) log_info, "Comando insert");
    funcionInsert(nombreDeTabla, atoi(key), value);

}

void comando_create(create_tad* create){
    print_console((void*) log_info, "Comando create");
    funcionCreate(create);
}

void comando_describe(char* tabla){
    funcionDescribe(tabla);
    print_console((void*) log_info, "Comando describe");
}

void comando_describe_all(origin){
    funcionDescribeAll(origin);
    print_console((void*) log_info, "Comando describe all");
}

void comando_drop(char* nombreTabla){
    print_console((void*) log_info, "Comando drop");
    funcionDrop(nombreTabla);
    // avisar al FS que haga el drop
}

void comando_journal(uint32_t servidorFileSystem){
    print_console((void*) log_info, "Comando journal");
    funcionJournal(servidorFileSystem);
}