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

void comando_create(create_tad* create, int requestOrigin){
    string_to_upper(create->nameTable);
    print_console((void*) log_info, "Comando create");
    log_info(log_Memoria,
             "CREATE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
             create->nameTable, create->consistencia, create->particiones, create->compactacion);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_CREATE);
    serializar_create(SERVIDOR_FILESYSTEM, create);

    if (requestOrigin != CONSOLE_REQUEST) {
        bool confirm = deserializar_int(SERVIDOR_FILESYSTEM);
        serializar_int(requestOrigin, confirm);
    }
}

void comando_describe(char* nombreTabla, int requestOrigin){
    print_console((void*) log_info, "Comando describe");
    string_to_upper(nombreTabla);
    log_info(log_Memoria, "DESCRIBE => TABLA: <%s>\t", nombreTabla);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_DESCRIBE);
    serializar_string(SERVIDOR_FILESYSTEM, nombreTabla);
    bool confirm = deserializar_int(SERVIDOR_FILESYSTEM);
    if (confirm) {
        describe_tad* describe = deserializar_describe(SERVIDOR_FILESYSTEM);
        if (requestOrigin != CONSOLE_REQUEST) {

            serializar_int(requestOrigin, true);
            serializar_describe(requestOrigin, describe);

        }

        log_info(log_Memoria,
                 "DESCRIBE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                 describe->nameTable, describe->consistencia, describe->particiones, describe->compactacion);

        free_describe_tad(describe);
    } else {

        serializar_int(SERVIDOR_FILESYSTEM, false);
    }

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