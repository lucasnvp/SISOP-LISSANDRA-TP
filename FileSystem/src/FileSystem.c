//
// Created by utnso on 06/04/19.
//

#include "FileSystem.h"

int main(){
    system("clear"); /* limpia la pantalla al empezar */

    puts("Proceso File System");

    //Inicializar Log
    init_log(PATH_LOG);

    //Configuracion inicial
    config = load_config(PATH_CONFIG);
    print_config(config, log_Console);

    //Creo el hilo del servidor
    pthread_create(&thread_server, NULL, (void*) server, "Servidor");

    //Creo el hilo de la consola
//    pthread_create(&thread_consola, NULL, (void*) consola, "Consola");

    pthread_join(thread_server, (void**) NULL);
//    pthread_join(thread_consola, (void**) NULL);

    return EXIT_SUCCESS;
}

void init_log(char* pathLog){
    mkdir("/home/utnso/Gank-mid/Logs",0755);
    log_Console = log_create(pathLog, "FileSystem", true, LOG_LEVEL_INFO);
    log_FileSystem = log_create(pathLog, "FileSystem", false, LOG_LEVEL_INFO);
}

void server(void* args) {
    fd_set read_fds; 	// conjunto temporal de descriptores de fichero para select()
    uint32_t fdmax;			// número máximo de descriptores de fichero
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
        default:
            log_info(log_FileSystem, "Error al recibir el comando");
    }
    return;
}
