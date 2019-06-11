//
// Created by utnso on 09/04/19.
//

#include "comandos.h"


void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Memoria, message);
    printf("%s", message);
}

void comando_select(uint32_t SERVIDOR_FILESYSTEM, select_tad* select, int requestOrigin){
    print_console((void*) log_info, "Comando select");
    char* selectValue = funcionSelect(SERVIDOR_FILESYSTEM, select->nameTable, select->key);
    if (selectValue == NULL) {

        select_tad* select = new_select_tad(select->nameTable, select->key);
        serializar_int(SERVIDOR_FILESYSTEM, COMAND_SELECT);
        serializar_select(SERVIDOR_FILESYSTEM, select);
        uint32_t confirm = deserializar_int(SERVIDOR_FILESYSTEM);
        if (confirm) {
            serializar_select(SERVIDOR_FILESYSTEM, select);
            char* value = deserializar_string(SERVIDOR_FILESYSTEM);
            funcionInsert(select->nameTable, select->key, value);

            if(requestOrigin != CONSOLE_REQUEST) {
                    serializar_int(requestOrigin, true);
                    serializar_string(requestOrigin, value);
                }
        } else {
            serializar_int(requestOrigin, false);
        }
    } else {
        log_info(log_Memoria, "SELECT => TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>",
                 select->nameTable,
                 select->key,
                 selectValue);
        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, true);
            serializar_string(requestOrigin, selectValue);
        }
    }
}


void comando_insert(insert_tad* insert, int requestOrigin){
    print_console((void*) log_info, "Comando insert");
    funcionInsert(insert->nameTable, insert->key, insert->value);

}

void comando_create(create_tad* create, int requestOrigin){
    print_console((void*) log_info, "Comando create");
    funcionCreate(create);
}

void comando_describe(char* tabla, int requestOrigin){
    funcionDescribe(tabla);
    print_console((void*) log_info, "Comando describe");
}

void comando_describe_all(int requestOrigin){
    funcionDescribeAll(requestOrigin);
    print_console((void*) log_info, "Comando describe all");
}

void comando_drop(char* nombreTabla, int requestOrigin){
    print_console((void*) log_info, "Comando drop");
    funcionDrop(nombreTabla);
    // avisar al FS que haga el drop
}

void comando_journal(uint32_t servidorFileSystem, int requestOrigin){
    print_console((void*) log_info, "Comando journal");
    funcionJournal(servidorFileSystem);
}