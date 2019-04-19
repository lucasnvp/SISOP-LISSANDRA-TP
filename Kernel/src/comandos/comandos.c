//
// Created by utnso on 09/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Kernel, message);
    printf("%s", message);
}

void comando_select(u_int32_t socket, char* tabla, u_int16_t key){
    print_console((void*) log_info, "Comando select");
    serializar_int(socket, COMAND_SELECT);
    // todo Envio la info a la memoria
    // todo Recibo el value
    log_info(log_Kernel, "SELECT => TABLA: <%s>\tkey: <%d>\tvalue: DUMMY_VALUE", tabla, key);
}

void comando_insert(u_int32_t socket, char* tabla, u_int16_t key, char* value){
    print_console((void*) log_info, "Comando insert");
    serializar_int(socket, COMAND_INSERT);
    // todo Envio la info a la memoria
    // todo Recibo la confirmacion
    log_info(log_Kernel, "INSERT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", tabla, key, value);
}

void comando_create(u_int32_t socket, char* tabla, char* consistencia, u_int32_t particiones, u_int32_t compactacion){
    print_console((void*) log_info, "Comando create");
    serializar_int(socket, COMAND_CREATE);
    // todo Envio la info a la memoria
    // todo Recibo la confirmacion
    log_info(log_Kernel,
            "CREATE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
            tabla, consistencia, particiones, compactacion);
}

void comando_describe(u_int32_t socket, char* tabla){
    print_console((void*) log_info, "Comando describe");
    serializar_int(socket, COMAND_DESCRIBE);
    // todo Envio la info a la memoria
    // todo Confirmacion de la operacion
    log_info(log_Kernel, "DESCRIBE - TABLA: <%s>\t", tabla);
}

void comando_drop(u_int32_t socket, char* tabla){
    print_console((void*) log_info, "Comando drop");
    serializar_int(socket, COMAND_DROP);
    // todo Envio la info a la memoria
    // todo Confirmacion de la operacion
    log_info(log_Kernel, "DROP - TABLA: <%s>\t", tabla);
}

void comando_run(char* path){
    if(ValidarArchivo(path)){
        print_console((void*) log_info, "Comando Run");
    } else{
        print_console((void*) log_info, "No se encontro el archivo");
        log_warning(log_Kernel, "No se encontro el archivo");
    }
}

void comando_metrics(){
    print_console((void*) log_info, "Comando metrics");
}