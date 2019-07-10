//
// Created by utnso on 09/04/19.
//

#include "comandos.h"


void comando_select(select_tad* select, int requestOrigin){
    registro_tad* registro = funcionSelect(requestOrigin, select);

    if (registro == NULL) {
      registro = solicitarSelectAFileSystem(requestOrigin, select);
    }

    // vuelvo a preguntar si es null para enviar la señal serializada (por si no existe la key solicitada en FS)

    if (registro == NULL) {
        if (requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, false);
        }
    } else {
        log_info(log_Memoria, "SELECT => TABLA: <%s>\tKEY: <%d>\tVALUE: <%s>\tTIMESTAMP: <%lld>",
            select->nameTable,
            registro->key,
            registro->value,
            registro->timestamp);

        if (requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, true);
            serializar_registro(requestOrigin, registro);
        }

    }

}


void comando_insert(insert_tad* insert, int requestOrigin){
    sem_wait(&semaforoInsert);
    funcionInsert(requestOrigin, insert, true, 0);
    sem_post(&semaforoInsert);
}

void comando_journal(int requestOrigin){
    sem_wait(&semaforoDrop);
    sem_wait(&semaforoInsert);
    funcionJournal(requestOrigin);
    sem_post(&semaforoDrop);
    sem_post(&semaforoInsert);
}

void comando_create(create_tad* create, int requestOrigin){
    string_to_upper(create->nameTable);
    log_info(log_Memoria,
             "CREATE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
             create->nameTable, create->consistencia, create->particiones, create->compactacion);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_CREATE);
    serializar_create(SERVIDOR_FILESYSTEM, create);
    bool confirm = deserializar_int(SERVIDOR_FILESYSTEM);

    if (requestOrigin != CONSOLE_REQUEST) {
        serializar_int(requestOrigin, confirm);
    }
}

void comando_drop(char* nombreTabla, int requestOrigin){

    char* tabla = string_duplicate(nombreTabla);
    sem_wait(&semaforoDrop);
    funcionDrop(tabla);
    sem_post(&semaforoDrop);
    log_info(log_Memoria,
             "DROP EN MEMORIA => TABLA: <%s>\t",
             tabla);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_DROP);
    serializar_string(SERVIDOR_FILESYSTEM, tabla);
    bool confirm = deserializar_int(SERVIDOR_FILESYSTEM);

    if (requestOrigin != CONSOLE_REQUEST) {

       serializar_int(requestOrigin, confirm);
    }

    free(tabla);
}

void comando_gossip(uint32_t socket) {
    sendGossipingTable(socket);
}

void comando_describe(char* nombreTabla, int requestOrigin){
    char* tabla = string_duplicate(nombreTabla);
    string_to_upper(tabla);
    log_info(log_Memoria, "DESCRIBE => TABLA: <%s>\t", tabla);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_DESCRIBE);
    serializar_string(SERVIDOR_FILESYSTEM, tabla);
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
        if(requestOrigin != CONSOLE_REQUEST){
            serializar_int(requestOrigin, false);
        }
    }

    free(tabla);
}


void comando_describe_all(int requestOrigin) {

    serializar_int(SERVIDOR_FILESYSTEM, COMAND_DESCRIBE_ALL);
    t_list* listaDescribes = deserializar_describe_all(SERVIDOR_FILESYSTEM);
    log_info(log_Memoria, "Se recibio del FS el describe all, se envia al Kernel");

    void print_element_stack(void* element){
        describe_tad* describe = element;
        log_info(log_Memoria,
                 "DESCRIBE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                 describe->nameTable, describe->consistencia, describe->particiones, describe->compactacion);
    }

    list_iterate(listaDescribes, print_element_stack);
    if (requestOrigin != CONSOLE_REQUEST) {
        serializar_describe_all(requestOrigin, listaDescribes);
    }
    list_destroy(listaDescribes);

}

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Memoria, message);
    printf("%s", message);
}
