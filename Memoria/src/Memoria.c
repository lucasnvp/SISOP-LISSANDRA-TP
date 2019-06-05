//
// Created by utnso on 06/04/19.
//

#include "Memoria.h"
//main
int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso Memoria");

    //Inicializar Log
    init_log(PATH_LOG);

    //Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);

    //Conexion al servidor FileSystem
    connect_server_FileSystem();

    // Se crea el espacio para la memoria principal
    memoriaPrincipal = alocar_MemoriaPrincipal();


    // Inicializamos la tabla de segmentos

    primerRegistroDeSegmentos = NULL;

	//TODO Hilo de Gossiping


    //Creo el hilo del servidor
    pthread_create(&thread_server, NULL, (void*) server, "Servidor");

    //Creo el hilo de la consola
    pthread_create(&thread_consola, NULL, (void*) memory_console, "Consola");

//    pthread_join(thread_server, (void**) NULL);
    pthread_join(thread_consola, (void**) NULL);


    // Se elimina el espacio para la memoria principal

    //desalocar_MemoriaPrincipal(); TODO: Creo que no deberíamos desalocar la memoria en este punto, o en ningún punto

    return EXIT_SUCCESS;
}

/*
 * TODO 08/05
 *
 * Buscar en las páginas de los segmentos si se contiene el key
 * Enviar solicitud a FileSystem para obtener el valor solicitado y luego almacenarlo
 * Comprobar si la memoria está llena
 * Insert Select Drop Describe
 * Journal, Gossiping
 */


void recibir_valores_FileSystem(uint32_t servidorFileSystem) {
    tamanoValue = deserializar_int(servidorFileSystem);
    tiempoDump = deserializar_int(servidorFileSystem);
}


registro_tad* alocar_MemoriaPrincipal() {
    registro_tad* aux = malloc(config.TAM_MEM);
    log_info(log_Memoria, "Se ha alocado la memoria principal");
    inicializarMarcos(config.TAM_MEM);
    log_info(log_Memoria, "Se han inicializado los marcos");
    return aux;
}

void desalocar_MemoriaPrincipal() {
    log_info(log_Memoria, "Se ha desalocado la memoria principal");
    free(memoriaPrincipal);
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "Memoria", true, LOG_LEVEL_INFO);
    log_Memoria = log_create(pathLog, "Memoria", false, LOG_LEVEL_INFO);
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

		//TODO aca se reciben los comandos de lo que se conecte a la memoria

        case NUEVA_CONEXION_KERNEL_TO_MEMORIA: {
            log_info(log_Memoria, "Se conecto el kernel");
            serializar_int(socket, config.MEMORY_NUMBER);
            break;
        }
        case COMAND_INSERT: {
            insert_tad* insert = deserializar_insert(socket);
            log_info(log_Memoria, "INSERT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", insert->nameTable, insert->key, insert->value);
            funcionInsert(insert->nameTable, insert->key, insert->value);
            break;
        }
        case COMAND_SELECT: {
            select_tad* select = deserializar_select(socket);
            char* value = funcionSelect(SERVIDOR_FILESYSTEM, select->nameTable, select->key);
            serializar_string(socket, value);
            break;
        }
        case COMAND_CREATE: {
            log_info(log_Memoria, "El kernel envio un create");
            create_tad* create = deserializar_create(socket);
            log_info(log_Memoria,
                     "CREATE => TABLA: <%s>\tCONSISTENCIA: <%s>\tPARTICIONES: <%d>\tCOMPACTACION: <%d>",
                     create->nameTable, create->consistencia, create->particiones, create->compactacion);
            serializar_int(SERVIDOR_FILESYSTEM, COMAND_CREATE);
            serializar_create(SERVIDOR_FILESYSTEM, create);
            free_create_tad(create);
            bool confirm = deserializar_int(SERVIDOR_FILESYSTEM);
            serializar_int(socket, confirm);
            break;
        }
        case COMAND_DESCRIBE: {
            log_info(log_Memoria, "El kernel envio un describe");
            char* tabla = deserializar_string(socket);
            log_info(log_Memoria, "DESCRIBE => TABLA: <%s>\t", tabla);
            free(tabla);
            break;
        }
        case COMAND_DROP: {
            log_info(log_Memoria, "El kernel envio un drop");
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
                    puts("Reconoci el comando");
                    puts(comandos->arg[0]);
                    puts(comandos->arg[1]);
                    comando_select(SERVIDOR_FILESYSTEM, comandos->arg[0], atoi(comandos->arg[1]));
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "insert")) {
                if (comandos->cantArgs == 3) {
                    puts("Reconoci el comando");
                    puts(comandos->arg[0]);
                    puts(comandos->arg[1]);
                    puts(comandos->arg[2]);
                    comando_insert(comandos->arg[0],comandos->arg[1],comandos->arg[2]);
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

            else print_console((void*) log_error, "Comando incorrecto.");

            // Libero toda la memoria
            for (i = 0; i < comandos->cantArgs; i++)
                free(comandos->arg[i]);

            free(comandos->comando);
        }
        free(linea);
    }
}