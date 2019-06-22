//
// Created by utnso on 06/04/19.
//

#include "Memoria.h"
//main
int main(int argc, char *argv[]){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Memoria");

    //Cargo los argumentos
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

    // Inicializamos los semáforos
    inicializarSemaforos();

    // Conexion al servidor FileSystem
    connect_server_FileSystem();

    // Se crea el espacio para la memoria principal
    memoriaPrincipal = alocar_MemoriaPrincipal();


    // Inicializamos la tabla de segmentos

    primerRegistroDeSegmentos = NULL;

    // Inicializamos los hilos de journal, gossiping, servidor y consola
    inicializarHilos();

//    pthread_join(thread_server, (void**) NULL);
    pthread_join(thread_consola, (void**) NULL);


    return EXIT_SUCCESS;
}


void journaling(){
    struct timeval timeJournal;

    while(true){

        timeJournal.tv_sec = 0;
        timeJournal.tv_usec = config.RETARDO_JOURNAL;

        select(0, NULL, NULL, NULL, &timeJournal);
        sem_wait(&semaforoDrop);
        sem_wait(&semaforoInsert);
        funcionJournal(SERVIDOR_FILESYSTEM);
        sem_post(&semaforoDrop);
        sem_post(&semaforoInsert);
    }
}

void gossiping(){
    struct timeval timeGossip;

    inicializarTablaDeGossiping();

    while(true){

        timeGossip.tv_sec = 0;
        timeGossip.tv_usec = (config.RETARDO_GOSSIPING * 1000);

        select(0, NULL, NULL, NULL, &timeGossip);

        funcionGossip();
    }

}

void recibir_valores_FileSystem(uint32_t servidorFileSystem) {
    tamanoValue = deserializar_int(servidorFileSystem);
}


registro_tad* alocar_MemoriaPrincipal() {
    inicializarMarcos(config.TAM_MEM);
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
    SERVIDOR_FILESYSTEM = connect_server(config.IP_FS,config.PUERTO_FS);

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
    uint32_t SERVIDOR_MEMORIA = build_server(config.PUERTO_ESCUCHA, config.CANT_CONEXIONES);

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
            memory_info_tad* memoryInfo = new_memory_info_tad(config.MEMORY_NUMBER, config.RETARDO_GOSSIPING);
            serializar_memory_info(socket, memoryInfo);
            free_memory_info_tad(memoryInfo);
            break;
        }
        case COMAND_INSERT: {
            log_info(log_Memoria, "El kernel envio un insert");
            insert_tad* insert = deserializar_insert(socket);
            log_info(log_Memoria, "INSERT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", insert->nameTable, insert->key, insert->value);
            comando_insert(insert, socket);
            free_insert_tad(insert);
            break;
        }
        case COMAND_SELECT: {
            log_info(log_Memoria, "El kernel envio un select");
            select_tad* select = deserializar_select(socket);
            comando_select(select, socket);
            free_select_tad(select);
            break;
        }
        case COMAND_CREATE: {
            log_info(log_Memoria, "El kernel envio un create");
            create_tad* create = deserializar_create(socket);
            comando_create(create, socket);
            free_create_tad(create);
            break;
        }
        case COMAND_DESCRIBE: {
            log_info(log_Memoria, "El kernel envio un describe");
            char* tabla = deserializar_string(socket);
            comando_describe(tabla, socket);
            free(tabla);
            break;
        }
        case COMAND_DESCRIBE_ALL: {
            log_info(log_Memoria, "El kernel envio un describe all");
            serializar_int(SERVIDOR_FILESYSTEM, COMAND_DESCRIBE_ALL);

            t_list* listDummy = deserializar_describe_all(SERVIDOR_FILESYSTEM);
            log_info(log_Memoria, "Se recibio del FS el describe all, se envia al Kernel");

            void print_element_stack(void* element){
                describe_tad* describe = element;
                log_info(log_Memoria,
                         "DESCRIBE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                         describe->nameTable, describe->consistencia, describe->particiones, describe->compactacion);
            }

            list_iterate(listDummy, print_element_stack);

            serializar_describe_all(socket, listDummy);
            list_destroy(listDummy);
            break;
        }
        case COMAND_DROP: {
            log_info(log_Memoria, "El kernel envio un drop");
            char* tabla = deserializar_string(socket);
            comando_drop(tabla, socket);
            free(tabla);


            break;
        }

        case COMAND_JOURNAL: {
            log_info(log_Memoria, "El kernel envio un journal");
            comando_journal(socket);
            break;
        }

        case COMAND_GOSSIP: {
            log_info(log_Memoria, "Se solicito la Tabla de Gossiping");
            comando_gossip(socket);
            break;
        }

        case COMAND_GOSSIP_RECEIVED: {
            log_info(log_Memoria, "Se recibe la Tabla de Gossiping de la otra memoria");
            t_list* gossipOtherMemory = deserializar_gossip_table(socket);
            compararTablasGossip(gossipOtherMemory);
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
                comandos->arg[i] = (char*) malloc (sizeof(char) * strlen(com));
                strcpy(comandos->arg[i], com);
                comandos->cantArgs++;
                com = strtok(NULL, " ");
                i++;
            }
            free(com);
            //TODO Case con cada uno de los comandos que acepta la consola de memoria
            if (!strcmp(comandos->comando, "exit")) {
                if (comandos->cantArgs == 0) {
                    free(comandos->comando);
                    break;
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "select")) {
                if (comandos->cantArgs == 2) {
                    select_tad* select = new_select_tad(comandos->arg[0], atoi(comandos->arg[1]));
                    comando_select(select, CONSOLE_REQUEST);
                    free_select_tad(select);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "insert")) {
                if (comandos->cantArgs == 3) {
                    insert_tad* insert = new_insert_tad(comandos->arg[0],atoi(comandos->arg[1]),comandos->arg[2]);
                    comando_insert(insert, CONSOLE_REQUEST);
                    free_insert_tad(insert);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "create")) {
                if (comandos->cantArgs == 4) {
                    create_tad* create = new_create_tad(comandos->arg[0], comandos->arg[1], atoi(comandos->arg[2]), atoi(comandos->arg[3]));
                    comando_create(create, CONSOLE_REQUEST);
                    free_create_tad(create);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "describe")) {
                if (comandos->cantArgs == 1) {
                    comando_describe(comandos->arg[0], CONSOLE_REQUEST);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "journal")) {
                if (comandos->cantArgs == 0) {
                    comando_journal(CONSOLE_REQUEST);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "drop")) {
                if (comandos->cantArgs == 1) {
                    comando_drop(comandos->arg[0], CONSOLE_REQUEST);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "help")) {
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
                    printGossip(config.MEMORY_NUMBER);
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
    sem_init(&semaforoDrop,0,1);
    sem_init(&semaforoInsert,0,1);
}

void inicializarHilos() {

    //Hilo de Journal
    pthread_create(&thread_journaling, NULL, (void*) journaling,"Hilo de Journal");

    //Hilo de Gossiping
    pthread_create(&thread_gossiping, NULL, (void*) gossiping, "Hilo de Gossiping");

    //Hilo del Servidor
    pthread_create(&thread_server, NULL, (void*) server, "Servidor");

    //Hilo de la Consola
    pthread_create(&thread_consola, NULL, (void*) memory_console, "Consola");
}