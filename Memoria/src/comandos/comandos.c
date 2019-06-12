//
// Created by utnso on 09/04/19.
//

#include "comandos.h"


void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Memoria, message);
    printf("%s", message);
}

char* comando_select(uint32_t SERVIDOR_FILESYSTEM, select_tad* select){
    print_console((void*) log_info, "Comando select");
    funcionSelect(SERVIDOR_FILESYSTEM, select);
}

void comando_insert(insert_tad* insert){
    print_console((void*) log_info, "Comando insert");
    funcionInsert(insert, true);

}

void comando_create(){
    print_console((void*) log_info, "Comando create");
}

void comando_describe(){
    print_console((void*) log_info, "Comando describe");
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