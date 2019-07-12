//
// Created by utnso on 06/04/19.
//

#include "Kernel.h"

int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Kernel");
    KERNEL_READY = true;

    // Inicializar Log
    init_log(PATH_LOG);

    // Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);
    configFilePathSize = string_length(PATH_CONFIG);

    // Inicializar Queue y Semaforos
    init_queue_and_sem();

    // Init listado de memorias
    init_memories();

    // Init listado de criterios
    init_criterios();

    // Init listado de criterios
    init_METADATA();

    // Init listado de metricas
    init_metrics();

    // Connect memory
    connect_memory(config->IP_MEMORIA, config->PUERTO_MEMORIA);

    //Creo el hilo de la consola
    pthread_create(&thread_consola, NULL, (void*) consola, "Consola");

    // Hilo de config
    pthread_create(&thread_config, NULL, (void*) watching_config, "WatchingConfig");

    // Hilo de metricas
    pthread_create(&thread_metricas, NULL, (void*) metricas, "Metricas");

    // Hilo de gossiping
    pthread_create(&thread_metricas, NULL, (void*) gossiping, "Gossiping");

    // Hilo de Planificacion
    pthread_create(&thread_planificador, NULL, (void*) planificador, "Planificador");

    // Espero el comando exit para finalizar el proceso.
    pthread_join(thread_consola, (void**) NULL);

    return EXIT_SUCCESS;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "Kernel", true, LOG_LEVEL_INFO);
    log_Kernel = log_create(pathLog, "Kernel", false, LOG_LEVEL_INFO);
    log_Kernel_api = log_Kernel;
    log_Kernel_memory = log_Kernel;
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
                comandos->arg[i] = strdup(com);
                comandos->cantArgs++;
                com = strtok(NULL, " ");
                i++;
            }
            free(com);

            string_to_upper(comandos->comando);

            if (!strcmp(comandos->comando, "EXIT")) {
                if (comandos->cantArgs == 0) {
                    KERNEL_READY = false;
                    free(comandos->comando);
                    break;
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "SELECT")) {
                if (comandos->cantArgs == 2) {
                    pthread_mutex_lock(&mutexGossip);
                    comando_select(comandos->arg[0], atoi(comandos->arg[1]));
                    pthread_mutex_unlock(&mutexGossip);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "INSERT")) {
                if (comandos->cantArgs == 3) {
                    pthread_mutex_lock(&mutexGossip);
                    comando_insert(comandos->arg[0], atoi(comandos->arg[1]), comandos->arg[2]);
                    pthread_mutex_unlock(&mutexGossip);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "CREATE")) {
                if (comandos->cantArgs == 4) {
                    pthread_mutex_lock(&mutexGossip);
                    comando_create(
                            comandos->arg[0],
                            comandos->arg[1],
                            atoi(comandos->arg[2]),
                            atoi(comandos->arg[3]));
                    pthread_mutex_unlock(&mutexGossip);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "DESCRIBE")) {
                if (comandos->cantArgs == 0) {
                    pthread_mutex_lock(&mutexGossip);
                    comando_describe_all();
                    pthread_mutex_unlock(&mutexGossip);
                } else {
                    if (comandos->cantArgs == 1) {
                        pthread_mutex_lock(&mutexGossip);
                        comando_describe(comandos->arg[0]);
                        pthread_mutex_unlock(&mutexGossip);
                    }
                    else print_console((void*) log_error, "Número de parámetros incorrecto.");
                }
            }

            else if (!strcmp(comandos->comando, "DROP")) {
                if (comandos->cantArgs == 1) {
                    pthread_mutex_lock(&mutexGossip);
                    comando_drop(comandos->arg[0]);
                    pthread_mutex_unlock(&mutexGossip);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "RUN")) {
                if (comandos->cantArgs == 1) {
                    char* path = string_duplicate(comandos->arg[0]);
                    comando_run(path, QUEUE_READY, &SEM_PLANIFICADOR);
                    free(path);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "METRICS")) {
                if (comandos->cantArgs == 0) {
                    showMetrics(log_Kernel);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "ADD")) {
                if (comandos->cantArgs == 4) {
                    if (string_equals_ignore_case(comandos->arg[0], "MEMORY") &&
                        string_equals_ignore_case(comandos->arg[2], "TO")) {
                        if (criterio_add(atoi(comandos->arg[1]), comandos->arg[3])) {
                            print_console((void*) log_info, "Criterio asignado a la memoria");
                        } else {
                            print_console((void*) log_info, "Error al asignar una memoria a un criterio");
                        }
                    } else print_console((void*) log_error, "No se encontró la orden - MEMORY or TO");
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "JOURNAL")) {
                if (comandos->cantArgs == 0) {
                    pthread_mutex_lock(&mutexGossip);
                    bool confirm = api_journal();
                    pthread_mutex_unlock(&mutexGossip);
                    if (confirm) {
                        print_console((void*) log_info, "Journal ejecutado correctamente");
                    }
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "PRINT_MEMORIES")) {
                if (comandos->cantArgs == 0) {
                    print_memories();
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

    pthread_mutex_init(&mutexMetrics, NULL);   // Inicializo el mutex de metricas
    pthread_mutex_init(&mutexConfig, NULL);     // Inicializo el mutex de config
    pthread_mutex_init(&mutexGossip, NULL);     // Inicializo el mutex de gossip

    sem_init(&SEM_PLANIFICADOR,0,0);	    // Hay procesos para Planificar
    sem_init(&SEM_MULTIPROGRAMACION,0,config->MULTIPROCESAMIENTO);	// Controlo la multiprogramacion
}

void metricas(){
    while(KERNEL_READY){
        sleep(30);
        showMetrics(log_Kernel);
    }
}

void execute(){
    script_tad* scripToRun = (script_tad*)queue_pop(QUEUE_READY);
    log_info(log_Kernel, "EXECUTE - Path to run: %s", scripToRun->path);

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    bool endOfScript = false;

    for (int k = 0; k < config->QUANTUM; ++k) {
        // Retardo de operacion
        usleep(config->RETARDO * 100);

        if ((read = getline(&line, &len, scripToRun->fp)) != -1) {
            if (!parser_line(line)){
                endOfScript = true;
                break;
            }
            scripToRun->lineas_ejecutadas = scripToRun->lineas_ejecutadas + 1;
        } else {
            endOfScript = true;
            break;
        }
    }

    if (endOfScript) {
        log_info(log_Kernel, "Se termino de ejecutar el scritp: %s", scripToRun->path);
        fclose(scripToRun->fp);
        if (line) {
            free(line);
        }
        // Fin del programa
        queue_push(QUEUE_EXIT, scripToRun);
    } else {
        // Lo agrego a la cola de READY
        queue_push(QUEUE_READY, scripToRun);
        // Nuevo script para planificar
        sem_post(&SEM_PLANIFICADOR);
    }

    // Multiprogramacion
    sem_post(&SEM_MULTIPROGRAMACION);

    pthread_exit(NULL);
}

void watching_config(){
    bufferInotifySize = sizeof(struct inotify_event) + configFilePathSize + 1;

    while (KERNEL_READY){
        fd_inotify = inotify_init();

        if (fd_inotify < 0) {
            log_error(log_Kernel, "inotify_init");
        }

        wd_inotify = inotify_add_watch(fd_inotify, PATH_CONFIG, IN_MODIFY);

        struct inotify_event* event = malloc(bufferInotifySize);

        length_inotify = read(fd_inotify, event, bufferInotifySize);

        if (length_inotify < 0) {
            log_error(log_Kernel, "Read");
        }

        if (event->mask == IN_MODIFY) {
            pthread_mutex_lock(&mutexConfig);
            free_config(config);
            config = load_config(PATH_CONFIG);
            print_config(config, log_Kernel);
            pthread_mutex_unlock(&mutexConfig);
        }

        free(event);

        (void) inotify_rm_watch(fd_inotify, wd_inotify);
        (void) close(fd_inotify);
    }
}

void planificador(){
    while (KERNEL_READY) {
        // Hay scripts para planificar
        sem_wait(&SEM_PLANIFICADOR);

        // Multiprogramacion
        sem_wait(&SEM_MULTIPROGRAMACION);

        // Ejecuto el Hilo de ejecucion
        pthread_create(&thread_exec, NULL, (void*) execute, "Ejecutar");

        // Espero que finalice el hilo de ejecucion
        pthread_detach(thread_exec);
    }
}

bool parser_line(char * line){
    // Parseo la linea
    t_lql_operacion parsed = parse(line);

    bool linea_valida = true;

    if(parsed.valido){
        switch(parsed.keyword){
            case SELECT:
                pthread_mutex_lock(&mutexGossip);
                api_select(parsed.argumentos.SELECT.tabla, parsed.argumentos.SELECT.key);
                pthread_mutex_unlock(&mutexGossip);
                break;
            case INSERT:
                pthread_mutex_lock(&mutexGossip);
                api_insert(parsed.argumentos.INSERT.tabla,
                        parsed.argumentos.INSERT.key,
                        parsed.argumentos.INSERT.value);
                pthread_mutex_unlock(&mutexGossip);
                break;
            case CREATE:
                pthread_mutex_lock(&mutexGossip);
                api_create(
                        parsed.argumentos.CREATE.tabla,
                        parsed.argumentos.CREATE.consistencia,
                        parsed.argumentos.CREATE.particiones,
                        parsed.argumentos.CREATE.compactacion);
                pthread_mutex_unlock(&mutexGossip);
                break;
            case DESCRIBE:
                if(parsed.argumentos.DESCRIBE.tabla == NULL) {
                    pthread_mutex_lock(&mutexGossip);
                    api_describe_all();
                    pthread_mutex_unlock(&mutexGossip);
                } else {
                    pthread_mutex_lock(&mutexGossip);
                    api_describe(parsed.argumentos.DESCRIBE.tabla);
                    pthread_mutex_unlock(&mutexGossip);
                }
                break;
            case DROP:
                pthread_mutex_lock(&mutexGossip);
                api_drop(parsed.argumentos.SELECT.tabla);
                pthread_mutex_unlock(&mutexGossip);
                break;
            default:
                log_info(log_Kernel, "No pude interpretar <%s>", line);
                linea_valida = false;
                break;
        }

        destruir_operacion(parsed);
    } else {
        log_info(log_Kernel, "La linea <%s> no es valida", line);
        linea_valida = false;
    }

    return linea_valida;
}

void gossiping(){
    while(KERNEL_READY){
        usleep(RETARDO_GOSSIPING * 1000);
        pthread_mutex_lock(&mutexGossip);
        gossip_memory();
        pthread_mutex_unlock(&mutexGossip);
    }
}