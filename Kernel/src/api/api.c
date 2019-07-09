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

        bool memoryFull = deserializar_int(socket);

        if (memoryFull) {
            bool confirm = api_journal();
            if (confirm) {
                log_info(log_Kernel_api, "Journal a todas las memorias.");

            }
        }

        uint32_t confirm = deserializar_int(socket);
        if (confirm) {
            registro_tad* registro = deserializar_registro(socket);
            log_info(log_Kernel_api, "SELECT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>\ttimestamp: <%d>",
                    tabla,
                    registro->key,
                    registro->value,
                    registro->timestamp);
            free_registro_tad(registro);
        } else {
            log_info(log_Kernel_api, "Error al recibir el SELECT");
        }
    }
}

void api_insert(char* tabla, u_int16_t key, char* value){
    uint32_t socket = get_memory_socket_from_metadata(tabla);

    if (socket == -1) {
        log_info(log_Kernel_api, "INSERT => La tabla: <%s> no existe", tabla);
    } else {
        serializar_int(socket, COMAND_INSERT);
        insert_tad * insert = new_insert_tad(tabla, key, value);
        serializar_insert(socket, insert);
        bool valueSize = deserializar_int(socket);
        if (valueSize) {
            bool memoryFull = deserializar_int(socket);
            if (memoryFull) {
                bool confirm = api_journal();
                if (confirm) {
                    serializar_int(socket, COMAND_INSERT);
                    serializar_insert(socket, insert);
                    valueSize = deserializar_int(socket);
                    memoryFull = deserializar_int(socket);
                }
            }
            log_info(log_Kernel_api, "INSERT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", tabla, key, value);
        } else {
            // todo cortar la ejecucion del script
            log_info(log_Kernel_api, "INSERT => FAILURE, Invalid value, TABLA: <%s>\tkey: <%d>\tvalue: <%s>", tabla, key, value);
        }

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
        create_tad* create = new_create_tad(tabla, consistencia, particiones, compactacion);
        serializar_create(socket, create);
        uint32_t confirm = deserializar_int(socket);
        if (confirm) {
            add_create_to_metadata(create);
            log_info(log_Kernel_api, "Se creo la tabla: %s, con exito", tabla);
        } else {
            log_info(log_Kernel_api, "Error al crear la tabla: %s", tabla);
        }
        free_create_tad(create);
    }
}

void api_describe(char* tabla){
    uint32_t socket = criterio_ramdom_memory_socket();

    if (socket == -1) {
        log_info(log_Kernel_api, "DESCRIBE => No se encontro memoria disponible");
    } else {
        serializar_int(socket, COMAND_DESCRIBE);
        serializar_string(socket, tabla);
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
        list_destroy_and_destroy_elements(listDescribe, free_describe_tad);
        print_metadata(log_Kernel_api);
    }
}

void api_drop(char* tabla){
    int socket = get_memory_socket_from_metadata(tabla);

    if (socket == -1) {
        log_info(log_Kernel_api, "DROP => La tabla: <%s> no existe", tabla);
    } else {
        serializar_int(socket, COMAND_DROP);
        serializar_string(socket, tabla);

        log_info(log_Kernel_api, "DROP => TABLA: <%s>\t", tabla);

        bool confirm = deserializar_int(socket);
        if (confirm) {
            drop_metadata(tabla);
            log_info(log_Kernel_api, "DROP => Se elimino la tabla con exito");
        } else {
            log_info(log_Kernel_api, "DROP => Fallo el request");
        }

    }
    // todo revisar free de tabla
}

bool api_journal () {
    return send_journal_all();
}