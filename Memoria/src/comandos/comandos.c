//
// Created by utnso on 09/04/19.
//

#include "comandos.h"


void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Memoria, message);
    printf("%s", message);
}


void comando_gossip(uint32_t requestOrigin) {
    sendGossipingTable(requestOrigin);
}

void comando_select(select_tad* select, int requestOrigin){
    registro_tad* registro = funcionSelect(select);

    if (registro == NULL) {
      registro = solicitarSelectAFileSystem(requestOrigin, select);
    }

    // vuelvo a preguntar si es null para enviar la señal serializada (por si no existe la key solicitada en FS)

    if (registro == NULL) {
        if (requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, false);
        }
    } else {
        log_info(log_Memoria, "SELECT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>\ttimestamp: <%d>",
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

void comando_describe(char* nombreTabla, int requestOrigin){
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

        //free_describe_tad(describe);

    } else {
        if(requestOrigin != CONSOLE_REQUEST){
            serializar_int(requestOrigin, false);
        }
    }

}

void comando_drop(char* nombreTabla, int requestOrigin){
    sem_wait(&semaforoDrop);
    funcionDrop(nombreTabla);
    sem_post(&semaforoDrop);
    serializar_int(SERVIDOR_FILESYSTEM, COMAND_DROP);
    serializar_string(SERVIDOR_FILESYSTEM, nombreTabla);
    bool confirm = deserializar_int(SERVIDOR_FILESYSTEM);

    if (requestOrigin != CONSOLE_REQUEST) {

       serializar_int(requestOrigin, confirm);
    }
}

void comando_journal(int requestOrigin){
    sem_wait(&semaforoDrop);
    sem_wait(&semaforoInsert);
    funcionJournal(requestOrigin);
    sem_post(&semaforoDrop);
    sem_post(&semaforoInsert);
}