//
// Created by utnso on 19/04/19.
//

#include "api.h"

void api_select(u_int32_t socket, char* tabla, u_int16_t key){
    serializar_int(socket, COMAND_SELECT);
    // todo Envio la info a la memoria
    // todo Recibo el value
    log_info(log_Kernel_api, "SELECT => TABLA: <%s>\tkey: <%d>\tvalue: DUMMY_VALUE", tabla, key);
}

void api_insert(u_int32_t socket, char* tabla, u_int16_t key, char* value){
    serializar_int(socket, COMAND_INSERT);
    // todo Envio la info a la memoria
    insert_tad* insert = new_insert_tad(tabla, key, value);
    serializar_insert(socket, insert);
    // todo Recibo la confirmacion
    log_info(log_Kernel_api, "INSERT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", tabla, key, value);
}

void api_create(u_int32_t socket, char* tabla, char* consistencia, u_int32_t particiones, u_int32_t compactacion){
    serializar_int(socket, COMAND_CREATE);
    // todo Envio la info a la memoria
    // todo Recibo la confirmacion
    log_info(log_Kernel_api,
             "CREATE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
             tabla, consistencia, particiones, compactacion);
}

void api_describe(u_int32_t socket, char* tabla){
    serializar_int(socket, COMAND_DESCRIBE);
    // todo Envio la info a la memoria
    // todo Confirmacion de la operacion
    log_info(log_Kernel_api, "DESCRIBE => TABLA: <%s>\t", tabla);
}

void api_drop(u_int32_t socket, char* tabla){
    serializar_int(socket, COMAND_DROP);
    // todo Envio la info a la memoria
    // todo Confirmacion de la operacion
    log_info(log_Kernel_api, "DROP => TABLA: <%s>\t", tabla);
}