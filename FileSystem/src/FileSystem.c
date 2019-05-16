//
// Created by utnso on 06/04/19.
//

#include "FileSystem.h"
#include "setup/setup.h"

int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso File System");

    //Inicializar Log
    init_log(PATH_LOG);

    //Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);

    //Config. inicial de FS
    punto_montaje_setup(config.PUNTO_MONTAJE);

    //Inicializa Memtable
    inicilizar_memtable();

    //Creo el hilo del servidor
    pthread_create(&thread_server, NULL, (void*) server, "Servidor");

    //Creo el hilo de la consola
    pthread_create(&thread_consola, NULL, (void*) consola, "Consola");

    //pthread_join(thread_server, (void**) NULL);
    pthread_join(thread_consola, (void**) NULL);

    return EXIT_SUCCESS;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "FileSystem", true, LOG_LEVEL_INFO);
    log_FileSystem = log_create(pathLog, "FileSystem", false, LOG_LEVEL_INFO);
}

void server(void* args) {
    fd_set read_fds; 	// conjunto temporal de descriptores de fichero para select()
    uint32_t fdmax;		// número máximo de descriptores de fichero
    int i;				// variable para el for
    FD_ZERO(&master);	// borra los conjuntos maestro
    FD_ZERO(&read_fds);	// borra los conjuntos temporal

    //Creacion del servidor
    uint32_t SERVIDOR_FILESYSTEM = build_server(config.PUERTO_ESCUCHA, config.CANT_CONEXIONES);

    //El socket esta listo para escuchar
    if(SERVIDOR_FILESYSTEM > 0) {
        log_info(log_FileSystem, "Servidor FileSystem Escuchando");
    }

    // añadir listener al conjunto maestro
    FD_SET(SERVIDOR_FILESYSTEM, &master);

    // seguir la pista del descriptor de fichero mayor
    fdmax = SERVIDOR_FILESYSTEM; // por ahora es éste

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
                if (i == SERVIDOR_FILESYSTEM) {
                    // acepto una nueva conexion
                    uint32_t newfd = accept_conexion(SERVIDOR_FILESYSTEM);
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
        case NUEVA_CONEXION: {
            log_info(log_FileSystem, "Se realizo una nueva conexion");
            break;
        }
        case INSERT: {
            log_info(log_FileSystem, "Insert");
            break;
        }
        case CREATE: {
            //TODO Hacer deserializador de comando create

            //comando_create(table, consistencia, cantidad_particiones, compactacion,socket);
            log_info(log_FileSystem, "Create");
            break;
        }
        default:
            log_info(log_FileSystem, "Error al recibir el comando");
    }
    return;
}

void consola() {
    char * com;
    char * linea;
    t_comandos * comandos;

    while (true) {
        comandos = (t_comandos*) malloc (sizeof(t_comandos));
        linea = readline("\nFile_System> ");

        if (strlen(linea) > 0) {
            log_info(log_FileSystem, "Linea de Consola: %s", linea);
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

            string_to_upper(comandos->comando);

            if (!strcmp(comandos->comando, "EXIT")) {
                if (comandos->cantArgs == 0) {
                    free(comandos->comando);
                    break;
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto. \n");
            }

            else if (!strcmp(comandos->comando, "SELECT")) {
                if (comandos->cantArgs == 2) {
                    char* table = comandos->arg[0];
                    char* key_string = comandos->arg[1];
                    int key = atoi(key_string);
                    comando_select(table,key);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto. \n");
            }

            else if (!strcmp(comandos->comando, "INSERT")) {
                if (comandos->cantArgs == 4) {
                    char* table = comandos->arg[0];
                    char* key_string = comandos->arg[1];
                    char* value = comandos->arg[2];
                    char* timestamp_string = comandos->arg[3];

                    int key = atoi(key_string);
                    int timestamp = atoi(timestamp_string);

                    string_to_upper(table);
                    comando_insert(table, key, value, timestamp,-1);
                } else {

                    if (comandos->cantArgs == 3) {
                        char* table = comandos->arg[0];
                        char* key_string = comandos->arg[1];
                        char* value = comandos->arg[2];

                        int key = atoi(key_string);
                        int timestamp = (int) time(NULL);

                        string_to_upper(table);
                        comando_insert(table, key, value, timestamp,-1);
                    }
                    else print_console((void*) log_error, "Número de parámetros incorrecto. \n");
                }
            }

            else if (!strcmp(comandos->comando, "CREATE")) {
                if (comandos->cantArgs == 4) {
                    char* table = comandos->arg[0];
                    char* consistencia = comandos->arg[1];
                    char* cantidad_particiones = comandos->arg[2];
                    char* compactacion = comandos->arg[3];

                    comando_create(table, consistencia, cantidad_particiones, compactacion,-1);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto. \n");
            }

            else if (!strcmp(comandos->comando, "DESCRIBE")) {
                if (comandos->cantArgs == 0) {
                    comando_describe_all(-1);
                } else {
                    if (comandos->cantArgs == 1) {
                        char* table = comandos->arg[0];
                        comando_describe(table,-1);
                    }
                    else print_console((void*) log_error, "Número de parámetros incorrecto. \n");
                }
            }

            else if (!strcmp(comandos->comando, "DROP")) {
                if (comandos->cantArgs == 1) {
                    char* table = comandos->arg[0];
                    comando_drop(table,-1);
                }
                else print_console((void*) log_error, "Número de parámetros incorrecto.");
            }

            else if (!strcmp(comandos->comando, "HELP")) {
                if (comandos->cantArgs == 0) {
                    printf("---------------------------------------------------------------------------------------------------------\n");
                    printf("Comandos posibles\n");
                    printf("Nombre del comando: parámetros -> Descripción del comando\n");
                    printf("EXIT -> Termina el proceso\n");
                    printf("CREATE: nombre de tabla, consistencia(SC, SHC, bla), particiones, compactacion(segs) -> Crea una tabla\n");
                    printf("INSERT (simple): tabla, key, value -> Inserta un registro en la tabla\n");
                    printf("INSERT: tabla, key, value, timestamp -> Inserta un registro en la tabla\n");
                    printf("SELECT: tabla, key -> Lee un registro de la tabla\n");
                    printf("DROP (no implementado aún): tabla -> Elimina una tabla\n");
                    printf("DESCRIBE (simple) -> Muestra los config de todas las tablas\n");
                    printf("DESCRIBE: tabla -> Muestra los config de una tabla\n");
                    printf("HELP -> Lista los comandos existentes\n");
                    printf("---------------------------------------------------------------------------------------------------------\n");
                } else {
                    print_console((void*) log_error, "Número de parámetros incorrecto. \n");
                }
            }

            else print_console((void*) log_error, "Comando incorrecto.\n");

            // Libero toda la memoria
            for (i = 0; i < comandos->cantArgs; i++)
                free(comandos->arg[i]);

            free(comandos->comando);
        }
        free(linea);
    }
}