//
// Created by utnso on 06/04/19.
//

#include "Kernel.h"

int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Kernel");

    //Inicializar Log
    init_log(PATH_LOG);

    //Inicializar Queue y Semaforos
    init_queue_and_sem();

    //Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);

    //Conexion al servidor FileSystem
    connect_server_Memoria();

    //Creo el hilo de la consola
    pthread_create(&thread_consola, NULL, (void*) consola, "Consola");

    //Hilo de metricas
    pthread_create(&thread_metricas, NULL, (void*) metricas, "Consola");

    // Hilo de ejecucion
    pthread_create(&thread_exec, NULL, (void*) execute, "Consola");

    // El join estan comentados, para que funcione el comando exit.
//    pthread_join(thread_metricas, (void**) NULL);
//    pthread_join(thread_exec, (void**) NULL);
    pthread_join(thread_consola, (void**) NULL);

    return EXIT_SUCCESS;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "Kernel", true, LOG_LEVEL_INFO);
    log_Kernel = log_create(pathLog, "Kernel", false, LOG_LEVEL_INFO);
    log_Kernel_api = log_Kernel;
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
                if (comandos->cantArgs == 2) {
                    comando_select(SERVIDOR_MEMORIA, comandos->arg[0], atoi(comandos->arg[1]));
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "insert")) {
                if (comandos->cantArgs == 3) {
                    comando_insert(SERVIDOR_MEMORIA, comandos->arg[0], atoi(comandos->arg[1]), comandos->arg[2]);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "create")) {
                if (comandos->cantArgs == 4) {
                    comando_create(
                            SERVIDOR_MEMORIA,
                            comandos->arg[0],
                            comandos->arg[1],
                            atoi(comandos->arg[2]),
                            atoi(comandos->arg[3]));
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "describe")) {
                if (comandos->cantArgs == 1) {
                    comando_describe(SERVIDOR_MEMORIA, comandos->arg[0]);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "drop")) {
                if (comandos->cantArgs == 1) {
                    comando_drop(SERVIDOR_MEMORIA, comandos->arg[0]);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "run")) {
                if (comandos->cantArgs == 1) {
                    comando_run(comandos->arg[0], QUEUE_READY, &SEM_EXECUTE);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "metrics")) {
                if (comandos->cantArgs == 0) {
                    comando_metrics();
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

void init_queue_and_sem(){
    QUEUE_NEW = queue_create();
    QUEUE_READY = queue_create();
    QUEUE_EXIT = queue_create();
    QUEUE_EXEC = queue_create();

    sem_init(&SEM_EXECUTE,0,0);	//Hay procesos para ejecutar
}

void metricas(){
    while(true){
        sleep(5000000000);
        log_warning(log_Kernel, "Mostrar Metricas");
    }
}

void execute(){
    while(true){
        //Hay procesos para ejecutar
        sem_wait(&SEM_EXECUTE);
        log_info(log_Kernel, "EXECUTE");
        char* path_to_run = queue_pop(QUEUE_READY);
        log_info(log_Kernel, "Path to run: %s", path_to_run);

        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;

        fp = fopen(path_to_run, "r");

        while ((read = getline(&line, &len, fp)) != -1) {
            t_lql_operacion parsed = parse(line);

            if(parsed.valido){
                switch(parsed.keyword){
                    case SELECT:
                        api_select(SERVIDOR_MEMORIA, parsed.argumentos.SELECT.tabla, parsed.argumentos.SELECT.key);
                        break;
                    case INSERT:
                        api_insert(SERVIDOR_MEMORIA,
                                parsed.argumentos.INSERT.tabla,
                                parsed.argumentos.INSERT.key,
                                parsed.argumentos.INSERT.value);
                        break;
                    case CREATE:
                        api_create(
                                SERVIDOR_MEMORIA,
                                parsed.argumentos.CREATE.tabla,
                                parsed.argumentos.CREATE.consistencia,
                                parsed.argumentos.CREATE.particiones,
                                parsed.argumentos.CREATE.compactacion);
                        break;
                    case DESCRIBE:
                        api_describe(SERVIDOR_MEMORIA, parsed.argumentos.DESCRIBE.tabla);
                        break;
                    case DROP:
                        api_drop(SERVIDOR_MEMORIA, parsed.argumentos.SELECT.tabla);
                        break;
                    default:
                        fprintf(stderr, "No pude interpretar <%s>\n", line);
                }

                destruir_operacion(parsed);
            } else {
                fprintf(stderr, "La linea <%s> no es valida\n", line);
            }
        }

        fclose(fp);
        if (line)
            free(line);
    }
}