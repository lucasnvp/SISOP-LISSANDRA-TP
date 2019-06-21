//
// Created by utnso on 19/04/19.
//

#include "api.h"

void api_select (char* tabla, u_int16_t key) {
    uint32_t socket = get_memory_socket_from_metadata(tabla);

    if (socket == -1) {
        log_info(log_Kernel_api, "SELECT => La tabla: <%s> no existe", tabla);
    } else {
        serializar_int(socket, COMAND_SELECT);
        select_tad* select = new_select_tad(tabla, key);
        serializar_select(socket, select);

        uint32_t confirm = deserializar_int(socket);
        if (confirm) {
            char* value = deserializar_string(socket);
            log_info(log_Kernel_api, "SELECT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", tabla, key, value);
            free(value);
        } else {
            log_info(log_Kernel_api, "Error al recibir el SELECT");
        }
    }
}

void api_insert(char* tabla, u_int16_t key, char* value){
    uint32_t socket = get_memory_socket_from_metadata(tabla);

    if (socket == -1) {
        log_info(log_Kernel_api, "SELECT => La tabla: <%s> no existe", tabla);
    } else {
        serializar_int(socket, COMAND_INSERT);
        insert_tad *insert = new_insert_tad(tabla, key, value);
        serializar_insert(socket, insert);

        // Hacer journal si la memoria esta full
        bool memoryFull = deserializar_int(socket);
        if (memoryFull) {
            //todo Mandar a todas las memorias journal
            serializar_int(socket, COMAND_JOURNAL);
            //serializar_int(socket, COMAND_INSERT);
            //serializar_insert(socket, insert);
        }

        log_info(log_Kernel_api, "INSERT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", tabla, key, value);
    }
}

void api_create(char* tabla, char* consistencia, u_int32_t particiones, u_int32_t compactacion) {
    uint32_t socket = criterio_ramdom_memory_socket();

    if (socket == -1) {
        log_info(log_Kernel_api, "CREATE => No se encontro memoria disponible");
    } else {
        serializar_int(socket, COMAND_CREATE);
        log_info(log_Kernel_api,
                 "CREATE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                 tabla, consistencia, particiones, compactacion);
        create_tad *create = new_create_tad(tabla, consistencia, particiones, compactacion);
        serializar_create(socket, create);
        free_create_tad(create);
        uint32_t confirm = deserializar_int(socket);
        if (confirm) {
            log_info(log_Kernel_api, "Se creo la tabla: %s, con exito", tabla);
        } else {
            log_info(log_Kernel_api, "Error al crear la tabla: %s", tabla);
        }
    }
}

void api_describe(char* tabla){
    uint32_t socket = criterio_ramdom_memory_socket();

    if (socket == -1) {
        log_info(log_Kernel_api, "DESCRIBE => No se encontro memoria disponible");
    } else {
        serializar_int(socket, COMAND_DESCRIBE);
        serializar_string(socket, tabla);
        log_info(log_Kernel_api, "DESCRIBE => TABLA: <%s>\t", tabla);
        bool confirm = deserializar_int(socket);
        if (confirm) {
            describe_tad *describe = deserializar_describe(socket);
            log_info(log_Kernel_api,
                     "DESCRIBE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                     describe->nameTable, describe->consistencia, describe->particiones, describe->compactacion);
            free_describe_tad(describe);
        } else {
            log_info(log_Kernel_api, "La tabla: %s, no existe", tabla);
        }
        // todo confirmar si aca iria un free de la tabla
    }
}

void api_describe_all () {
    uint32_t socket = criterio_ramdom_memory_socket();

    if (socket == -1) {
        log_info(log_Kernel_api, "DESCRIBE ALL => No se encontro memoria disponible");
    } else {
        serializar_int(socket, COMAND_DESCRIBE_ALL);

        t_list* listDescribe = deserializar_describe_all(socket);
        load_METADATA(listDescribe);
        print_metadata(log_Kernel_api);
    }
}

void api_drop(char* tabla){
    uint32_t socket = get_memory_socket_from_metadata(tabla);

    if (socket == -1) {
        log_info(log_Kernel_api, "DROP => La tabla: <%s> no existe", tabla);
    } else {
        serializar_int(socket, COMAND_DROP);
        // todo Envio la info a la memoria
        // todo Confirmacion de la operacion
        log_info(log_Kernel_api, "DROP => TABLA: <%s>\t", tabla);
    }
}

void api_journal () {

}