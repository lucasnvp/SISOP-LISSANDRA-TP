//
// Created by utnso on 06/04/19.
//

#include "Kernel.h"

int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Kernel");

    //Inicializar Log
    init_log(PATH_LOG);

    //Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);

    //Conexion al servidor FileSystem
    connect_server_Memoria();

    //Creo el hilo de la consola
    pthread_create(&thread_consola, NULL, (void*) consola, "Consola");

    pthread_join(thread_consola, (void**) NULL);

    return EXIT_SUCCESS;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "Kernel", true, LOG_LEVEL_INFO);
    log_Kernel = log_create(pathLog, "Kernel", false, LOG_LEVEL_INFO);
}

void connect_server_Memoria(){
    //Conexion al servidor Memoria
    SERVIDOR_MEMORIA = connect_server(config.IP_MEMORIA,config.PUERTO_MEMORIA);

    //Si conecto, informo
    if(SERVIDOR_MEMORIA > 1){
        log_info(log_Console,"Connected successfully to the Memory");
    } else{
        log_warning(log_Console, "No se puedo conectar al servidor de Memoria");
        exit(EXIT_SUCCESS);
    }
}

void consola() {
    char * com;
    char * linea;
    t_comandos * comandos;

    while (true) {
        comandos = (t_comandos*) malloc (sizeof(t_comandos));
        linea = readline("\nKernel> ");

        if (strlen(linea) > 0) {
            log_info(log_Kernel, "Linea de Consola: %s", linea);
            add_history(linea);

            com = strtok(linea, " ");
            comandos->comando = (char*) malloc (sizeof(char) * strlen(com));
            strcpy(comandos->comando, com);
            comandos->cantArgs = 0;

            com = strtok(NULL, " ");
            uint32_t i = 0;
            while (i < 4 && com) {
                comandos->arg[i] = (char*) malloc (sizeof(char) * strlen(com));
                strcpy(comandos->arg[i], com);
                comandos->cantArgs++;
                com = strtok(NULL, " ");
                i++;
            }
            free(com);

            if (!strcmp(comandos->comando, "exit")) {
                if (comandos->cantArgs == 0) {
                    free(comandos->comando);
                    break;
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "select")) {
                if (comandos->cantArgs == 0) {
                    comando_select();
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "insert")) {
                if (comandos->cantArgs == 0) {
                    comando_insert();
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "create")) {
                if (comandos->cantArgs == 0) {
                    comando_create();
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "describe")) {
                if (comandos->cantArgs == 0) {
                    comando_describe();
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "drop")) {
                if (comandos->cantArgs == 0) {
                    comando_drop();
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "run")) {
                if (comandos->cantArgs == 1) {
                    comando_run(comandos->arg[0]);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else print_console((void*) log_error, "Comando incorrecto.");

            // Libero toda la memoria
            for (i = 0; i < comandos->cantArgs; i++)
                free(comandos->arg[i]);

            free(comandos->comando);
        }
        free(linea);
    }
}