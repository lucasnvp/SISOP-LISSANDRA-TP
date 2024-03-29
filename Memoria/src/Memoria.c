﻿//
// Created by utnso on 06/04/19.
//

#include "Memoria.h"
//main
int main(int argc, char *argv[]){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Memoria");

    //Cargo los argumentos
    if (argv[1] == NULL) {
        puts("Error, falta el path del archivo de config");
        return EXIT_FAILURE;
    }
    PATH_CONFIG = strdup(argv[1]);
    if (!ValidarArchivo(PATH_CONFIG)) {
        puts("Error en el path del archivo de config");
        return EXIT_FAILURE;
    }

    //Inicializar Log
    init_log(PATH_LOG);

    // Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);
    configFilePathSize = string_length(PATH_CONFIG);

    // Inicializamos los semáforos
    inicializarSemaforos();

    // Conexion al servidor FileSystem
    connect_server_FileSystem();

    // Se crea el espacio para la memoria principal
    memoriaPrincipal = alocar_MemoriaPrincipal();

    // Memory ready
    MEMORY_READY = true;

    // Inicializamos la tabla de segmentos
    primerRegistroDeSegmentos = NULL;

    // Inicializamos los hilos de journal, gossiping, servidor y consola
    inicializarHilos();

//    pthread_join(thread_server, (void**) NULL);
    pthread_join(thread_consola, (void**) NULL);


    return EXIT_SUCCESS;
}


void journaling(){
    while(MEMORY_READY){
        usleep(config->RETARDO_JOURNAL * 1000);
//        sem_wait(&semaforoDrop);
//        sem_wait(&semaforoInsert);
        pthread_mutex_lock(&mutexLock);
        funcionJournal(CONSOLE_REQUEST);
        pthread_mutex_unlock(&mutexLock);
//        sem_post(&semaforoDrop);
//        sem_post(&semaforoInsert);
    }
}

void gossiping(){
    inicializarTablaDeGossiping();

    while(MEMORY_READY){
        usleep(config->RETARDO_GOSSIPING * 1000);
        pthread_mutex_lock(&mutexLock);
        funcionGossip();
        pthread_mutex_unlock(&mutexLock);
    }

}

void recibir_valores_FileSystem(uint32_t servidorFileSystem) {
    tamanoValue = deserializar_int(servidorFileSystem);
}


registro_tad* alocar_MemoriaPrincipal() {
    inicializarMarcos(config->TAM_MEM);
    uint32_t tamanioMemoria = sizeof(registro_tad) * cantidadDeMarcos;
    registro_tad* aux = malloc(tamanioMemoria);
    log_info(log_Memoria, "Se ha alocado la memoria principal");
    log_info(log_Memoria, "Se han inicializado los marcos");
    return aux;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "Memoria", true, LOG_LEVEL_INFO);
    log_Memoria = log_create(pathLog, "Memoria", false, LOG_LEVEL_INFO);
    log_Memoria_gossip = log_Memoria;
}

void connect_server_FileSystem(){
    //Conexion al servidor FileSystem
    SERVIDOR_FILESYSTEM = connect_server(config->IP_FS,config->PUERTO_FS);

    //Si conecto, informo
    if(SERVIDOR_FILESYSTEM > 1){
        log_info(log_Console,"Connected successfully to the File System");
        serializar_int(SERVIDOR_FILESYSTEM, NUEVA_CONEXION_MEMORIA_TO_FS);
        tamanoValue = deserializar_int(SERVIDOR_FILESYSTEM);
    } else{
        log_info(log_Console, "No se puedo conectar al servidor de File System");
        exit(EXIT_SUCCESS);
    }
}

void server(void* args) {
    fd_set read_fds; 	// conjunto temporal de descriptores de fichero para select()
    uint32_t fdmax;			// número máximo de descriptores de fichero
    int i;				// variable para el for
    FD_ZERO(&master);	// borra los conjuntos maestro
    FD_ZERO(&read_fds);	// borra los conjuntos temporal

    //Creacion del servidor
    uint32_t SERVIDOR_MEMORIA = build_server(config->PUERTO_ESCUCHA, config->CANT_CONEXIONES);

    //El socket esta listo para escuchar
    if(SERVIDOR_MEMORIA > 0) {
        log_info(log_Memoria, "Servidor Memoria Escuchando");
    }

    // añadir listener al conjunto maestro
    FD_SET(SERVIDOR_MEMORIA, &master);

    // seguir la pista del descriptor de fichero mayor
    fdmax = SERVIDOR_MEMORIA; // por ahora es éste

    // bucle principal
    while(true) {
        read_fds = master; // cópialo
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }
        // explorar conexiones existentes en busca de datos que leer
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // ¡¡tenemos datos!!
                if (i == SERVIDOR_MEMORIA) {
                    // acepto una nueva conexion
                    uint32_t newfd = accept_conexion(SERVIDOR_MEMORIA);
                    FD_SET(newfd, &master); // añadir al conjunto maestro
                    if (newfd > fdmax) {    // actualizar el máximo
                        fdmax = newfd;
                    }
                } else {
                    //Recibo el comando
                    uint32_t command = deserializar_int(i);

                    // gestionar datos de un cliente
                    if(command <= 0) {
                        close(i); // Close conexion
                        FD_CLR(i, &master); // eliminar del conjunto maestro
                    } else {
                        usleep(config->RETARDO_MEM*1000);
                        connection_handler(i, command);
                    }
                }
            }
        }
    }
}

void connection_handler(uint32_t socket, uint32_t command){
    switch (command){

        case NUEVA_CONEXION_KERNEL_TO_MEMORIA: {
            log_info(log_Memoria, "Se conecto el kernel");
            memory_info_tad* memoryInfo = new_memory_info_tad(config->MEMORY_NUMBER, config->RETARDO_GOSSIPING);
            serializar_memory_info(socket, memoryInfo);
            free_memory_info_tad(memoryInfo);
            break;
        }
        case COMAND_INSERT: {
            log_info(log_Memoria, "El kernel envio un insert");
            pthread_mutex_lock(&mutexLock);
            insert_tad* insert = deserializar_insert(socket);
            log_info(log_Memoria, "INSERT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", insert->nameTable, insert->key, insert->value);
            comando_insert(insert, socket);
            pthread_mutex_unlock(&mutexLock);
            free_insert_tad(insert);
            break;
        }
        case COMAND_SELECT: {
            log_info(log_Memoria, "El kernel envio un select");
            pthread_mutex_lock(&mutexLock);
            select_tad* select = deserializar_select(socket);
            comando_select(select, socket);
            pthread_mutex_unlock(&mutexLock);
            free_select_tad(select);
            break;
        }
        case COMAND_CREATE: {
            log_info(log_Memoria, "El kernel envio un create");
            pthread_mutex_lock(&mutexLock);
            create_tad* create = deserializar_create(socket);
            comando_create(create, socket);
            pthread_mutex_unlock(&mutexLock);
            free_create_tad(create);
            break;
        }
        case COMAND_DESCRIBE: {
            log_info(log_Memoria, "El kernel envio un describe");
            pthread_mutex_lock(&mutexLock);
            char* tabla = deserializar_string(socket);
            comando_describe(tabla, socket);
            pthread_mutex_unlock(&mutexLock);
            free(tabla);
            break;
        }
        case COMAND_DESCRIBE_ALL: {
            log_info(log_Memoria, "El kernel envio un describe all");
            pthread_mutex_lock(&mutexLock);
            comando_describe_all(socket);
            pthread_mutex_unlock(&mutexLock);
            break;
        }
        case COMAND_DROP: {
            log_info(log_Memoria, "El kernel envio un drop");
            pthread_mutex_lock(&mutexLock);
            char* tabla = deserializar_string(socket);
            comando_drop(tabla, socket);
            pthread_mutex_unlock(&mutexLock);
            free(tabla);
            break;
        }

        case COMAND_JOURNAL: {
            log_info(log_Memoria, "El kernel envio un journal");
            pthread_mutex_lock(&mutexLock);
            comando_journal(socket);
            pthread_mutex_unlock(&mutexLock);
            break;
        }

        case COMAND_GOSSIP: {
            log_info(log_Memoria, "Se solicito la Tabla de Gossiping");
            //pthread_mutex_lock(&mutexLock);
            comando_gossip(socket);
            //pthread_mutex_unlock(&mutexLock);
            break;
        }

        case COMAND_GOSSIP_RECEIVED: {
            log_info(log_Memoria, "Se recibe la Tabla de Gossiping de la otra memoria");
            //pthread_mutex_lock(&mutexLock);
            t_list* gossipOtherMemory = deserializar_gossip_table(socket);
            compararTablasGossip(gossipOtherMemory);
            //pthread_mutex_unlock(&mutexLock);
            list_destroy_and_destroy_elements(gossipOtherMemory, free_gossip_tad);
            break;
        }

        default:
            log_info(log_Memoria, "Error al recibir el comando");
    }
    return;
}

void memory_console() {
    char * com;
    char * linea;
    t_comandos * comandos;

    while (true) {
        comandos = (t_comandos*) malloc (sizeof(t_comandos));
        linea = readline("\nMemoria> ");

        if (strlen(linea) > 0) {
            log_info(log_Memoria, "Linea de Consola: %s", linea);
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
                    free(comandos->comando);
                    break;
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "SELECT")) {
                if (comandos->cantArgs == 2) {
                    select_tad* select = new_select_tad(comandos->arg[0], atoi(comandos->arg[1]));
                    comando_select(select, CONSOLE_REQUEST);
                    free_select_tad(select);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "INSERT")) {
                if (comandos->cantArgs == 3) {
                    insert_tad* insert = new_insert_tad(comandos->arg[0],atoi(comandos->arg[1]),comandos->arg[2]);
                    comando_insert(insert, CONSOLE_REQUEST);
                    free_insert_tad(insert);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "CREATE")) {
                if (comandos->cantArgs == 4) {
                    create_tad* create = new_create_tad(comandos->arg[0], comandos->arg[1], atoi(comandos->arg[2]), atoi(comandos->arg[3]));
                    comando_create(create, CONSOLE_REQUEST);
                    free_create_tad(create);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "DESCRIBE")) {
                if (comandos->cantArgs == 0) {
                    comando_describe_all(CONSOLE_REQUEST);
                }

                else if (comandos->cantArgs == 1) {
                    comando_describe(comandos->arg[0], CONSOLE_REQUEST);
                }

                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "JOURNAL")) {
                if (comandos->cantArgs == 0) {
                    comando_journal(CONSOLE_REQUEST);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "DROP")) {
                if (comandos->cantArgs == 1) {
                    comando_drop(comandos->arg[0], CONSOLE_REQUEST);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "HELP")) {
                if (comandos->cantArgs == 0) {
                    printf("---------------------------------------------------------------------------------------------------------------------------\n");
                    printf("Comandos posibles\n");
                    printf("\n");
                    printf("Comando     Parámetros                                          -> Descripción del comando\n");
                    printf("\n");
                    printf("SELECT      <TABLA> <KEY>                                       -> Obtener el valor de una key dentro de una tabla\n");
                    printf("INSERT      <TABLA> <KEY> <VALUE>                               -> Crear/Actualizar el valor de una key dentro de una tabla\n");
                    printf("CREATE      <TABLA> <CONSISTENCIA> <PARTICIONES> <COMPACTACION> -> Crear una nueva tabla dentro del FileSystem\n");
                    printf("DESCRIBE    [<TABLA>]                                           -> Obtener la metadata de una tabla en particular\n");
                    printf("DROP        <TABLA>                                             -> Eliminar una tabla dentro del FileSystem\n");
                    printf("JOURNAL     <TABLA>                                             -> Envío de información de Memoria a FileSystem\n");
                    printf("HELP                                                            -> Lista los comandos existentes\n");
                    printf("---------------------------------------------------------------------------------------------------------------------------\n");
                } else {
                    print_console((void*) log_error, "Número de parámetros incorrecto. \n");
                }
            }

            else if (!strcmp(comandos->comando, "printGossip")) {
                if (comandos->cantArgs == 0) {
                    printGossip(config->MEMORY_NUMBER);
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

void inicializarSemaforos() {
    pthread_mutex_init(&mutexConfig, NULL); // Inicializo el mutex de config
    pthread_mutex_init(&mutexLock, NULL);   // Inicializo el mutex de lock
//    sem_init(&semaforoDrop,0,1);
//    sem_init(&semaforoInsert,0,1);
}

void inicializarHilos() {
    // Hilo del Servidor
    pthread_create(&thread_server, NULL, (void*) server, "Servidor");

    // Hilo de la Consola
    pthread_create(&thread_consola, NULL, (void*) memory_console, "Consola");

    // Hilo de config
    pthread_create(&thread_config, NULL, (void*) watching_config, "WatchingConfig");

    // Hilo de Journal
   // pthread_create(&thread_journaling, NULL, (void*) journaling,"Hilo de Journal");

    // Hilo de Gossiping
    pthread_create(&thread_gossiping, NULL, (void*) gossiping, "Hilo de Gossiping");
}

void watching_config(){
    bufferInotifySize = sizeof(struct inotify_event) + configFilePathSize + 1;

    while (MEMORY_READY){
        fd_inotify = inotify_init();

        if (fd_inotify < 0) {
            log_error(log_Memoria, "inotify_init");
        }

        wd_inotify = inotify_add_watch(fd_inotify, PATH_CONFIG, IN_MODIFY);

        struct inotify_event* event = malloc(bufferInotifySize);

        length_inotify = read(fd_inotify, event, bufferInotifySize);

        if (length_inotify < 0) {
            log_error(log_Memoria, "Read");
        }

        if (event->mask == IN_MODIFY) {
            pthread_mutex_lock(&mutexConfig);
            free_config(config);
            config = load_config(PATH_CONFIG);
            print_config(config, log_Memoria);
            pthread_mutex_unlock(&mutexConfig);
        }

        free(event);

        (void) inotify_rm_watch(fd_inotify, wd_inotify);
        (void) close(fd_inotify);
    }
}