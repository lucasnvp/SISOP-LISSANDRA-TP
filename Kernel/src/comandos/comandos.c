//
// Created by utnso on 09/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_Kernel, message);
    printf("%s", message);
}

void comando_select(char* tabla, u_int16_t key){
    print_console((void*) log_info, "Comando select");
    api_select(tabla, key);
}

void comando_insert(char* tabla, u_int16_t key, char* value){
    print_console((void*) log_info, "Comando insert");
    api_insert(tabla, key, value);
}

void comando_create(char* tabla, char* consistencia, u_int32_t particiones, u_int32_t compactacion){
    print_console((void*) log_info, "Comando create");
    api_create(tabla, consistencia, particiones, compactacion);
}

void comando_describe(char* tabla){
    print_console((void*) log_info, "Comando describe");
    api_describe(tabla);
}

void comando_describe_all(){
    print_console((void*) log_info, "Comando describe all");
    api_describe_all();
}

void comando_drop(char* tabla){
    print_console((void*) log_info, "Comando drop");
    api_drop(tabla);
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

void comando_journal() {
    print_console((void*) log_info, "Comando journal");
    api_journal();
}