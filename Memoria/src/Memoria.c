//
// Created by utnso on 06/04/19.
//

#include "Memoria.h"

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

    //TODO Crear el espacio de memoria leyendo la configuracion que es enviada por el file system

    //TODO Hilo de Gossiping

    //Creo el hilo del servidor
    pthread_create(&thread_server, NULL, (void*) server, "Servidor");

    //Creo el hilo de la consola
    pthread_create(&thread_consola, NULL, (void*) memory_console, "Consola");

//    pthread_join(thread_server, (void**) NULL);
    pthread_join(thread_consola, (void**) NULL);

    return EXIT_SUCCESS;
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

        //TODO Recibir tamano_value y tiempo_dump de File System
        //lo necesitamos para crear el espacio de la memoria (tamano_value) y el tiempo de dump

        log_info(log_Console,"Connected successfully to the File System");

    } else{
        log_warning(log_Console, "No se puedo conectar al servidor de File System");
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
        case NUEVA_CONEXION: {
            log_info(log_Memoria, "Se realizo una nueva conexion");
            break;
        }

        //TODO aca se reciben los comandos de lo que se conecte a la memoria

        case INSERT: {

            log_info(log_Memoria, "Insert");
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

            else print_console((void*) log_error, "Comando incorrecto.");

            // Libero toda la memoria
            for (i = 0; i < comandos->cantArgs; i++)
                free(comandos->arg[i]);

            free(comandos->comando);
        }
        free(linea);
    }
}