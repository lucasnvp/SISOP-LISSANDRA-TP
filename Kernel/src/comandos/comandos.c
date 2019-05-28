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
    api_select(socket, tabla, key);
}

void comando_insert(u_int32_t socket, char* tabla, u_int16_t key, char* value){
    print_console((void*) log_info, "Comando insert");
    api_insert(socket, tabla, key, value);
}

void comando_create(u_int32_t socket, char* tabla, char* consistencia, u_int32_t particiones, u_int32_t compactacion){
    print_console((void*) log_info, "Comando create");
    api_create(socket, tabla, consistencia, particiones, compactacion);
}

void comando_describe(u_int32_t socket, char* tabla){
    print_console((void*) log_info, "Comando describe");
    api_describe(socket, tabla);
}

void comando_drop(u_int32_t socket, char* tabla){
    print_console((void*) log_info, "Comando drop");
    api_drop(socket, tabla);
}

void comando_run(char* path, t_queue* QUEUE_READY, sem_t* SEM_PLANIFICADOR){
    if(ValidarArchivo(path)){
        print_console((void*) log_info, "Comando Run");
        // Nuevo script
        script_tad* newScript = script_new(path);
        // Lo agrego a la cola de READY
        queue_push(QUEUE_READY, newScript);
        // Nuevo script para planificar
        sem_post(SEM_PLANIFICADOR);
    } else{
        print_console((void*) log_info, "No se encontro el archivo");
        log_info(log_Kernel, "No se encontro el archivo");
    }
}

void comando_metrics(){
    print_console((void*) log_info, "Comando metrics");
}