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

    // Se crea el espacio para la memoria principal
    memoriaPrincipal = alocar_MemoriaPrincipal();


    // Inicializamos la tabla de segmentos

    tablaDeSegmentos = NULL;

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




/*** --------------- PÁGINAS --------------- ***/

// REGISTROS DE PÁGINAS



// Reserva un marco para una Página marcándolo como ocupado y devuelve su dirección de memoria
// Si todos los marcos se encuentran ocupados, se buscará una Página para Liberar

registo_tad* reservarMarco() {
    struct tablaDeMarcos* _tablaDeMarcos = tablaDeMarcos;
    while(_tablaDeMarcos != NULL){
        if(_tablaDeMarcos->registro.marcoOcupado == false) {
            _tablaDeMarcos->registro.marcoOcupado = true;
            return (registo_tad*)(memoriaPrincipal + _tablaDeMarcos->registro.numeroMarco * sizeof(registo_tad));
        }


        _tablaDeMarcos = _tablaDeMarcos->siguiente;
    }
    // si no se encontró ningun marco libre, se procede a liberar una página
    return liberarPagina();
}


// Agrega un registro de Página a la Tabla de Páginas
// --- registo_tad es la página
void agregarRegistroDePagina(tablaDePaginas* _tablaDePaginas, registo_tad* punteroAPagina) {
    tablaDePaginas* nuevoRegistroDePagina;
    nuevoRegistroDePagina->registro.flagModificado = false;
    nuevoRegistroDePagina->registro.numeroPagina = _tablaDePaginas->registro.numeroPagina + 1;
    nuevoRegistroDePagina->registro.punteroAPagina = punteroAPagina;
    nuevoRegistroDePagina->siguienteRegistroPagina = NULL;
    _tablaDePaginas->siguienteRegistroPagina = nuevoRegistroDePagina;
}

// Ocupa el marco de página
void ocuparPagina(registo_tad* punteroAPagina, uint32_t timestamp, uint32_t key, char* value ) {
    punteroAPagina = new_registro_tad(timestamp, key, value);
}

// Obtiene el registro más viejo de todos los segmentos disponibles en la memoria. En caso de no encontrar ninguno, ejecuta journal. //TODO: Journal
tablaDePaginas* obtenerRegistroMasViejo() {
    struct tablaDeSegmentos* _tablaDeSegmentos = tablaDeSegmentos;
    // el registro más viejo, es el primero que encuentro
    tablaDePaginas* registroMasViejo = _tablaDeSegmentos->registro.tablaDePaginas;
    // en tanto tenga segmentos para iterar
    while( _tablaDeSegmentos != NULL ){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // en tanto tenga páginas dentro de mi segmento
        while ( pagina != NULL){
            // compruebo si el último acceso es menor que el registroMasViejo, y de ser así, compruebo si la página contiene el flag de modificado en false
            if (pagina->registro.ultimoAcceso < registroMasViejo->registro.ultimoAcceso & !pagina->registro.flagModificado){
                registroMasViejo = pagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

    // en este punto no hay ninguna página en ningún segmento que tenga el flag de modificado activado; entonces compruebo el registro más viejo
    if(registroMasViejo == _tablaDeSegmentos->registro.tablaDePaginas & registroMasViejo->registro.flagModificado) {
        //ejecutar journal
    }
    return registroMasViejo;
}

// Reenlaza los registros de páginas
registo_tad* reenlazarRegistros(tablaDePaginas* registroMasViejo) {


    struct tablaDeSegmentos* _tablaDeSegmentos = tablaDeSegmentos;

    // en tanto tenga segmentos disponibles
    while(_tablaDeSegmentos != NULL){
        tablaDePaginas* pagina = _tablaDeSegmentos->registro.tablaDePaginas;
        // si el registro más viejo es la página del segmento, enlazo en la lista
        if(registroMasViejo == pagina){
            *(_tablaDeSegmentos)->registro.tablaDePaginas = *(registroMasViejo)->siguienteRegistroPagina;
            return registroMasViejo->registro.punteroAPagina;
        }
        // en tanto tenga páginas disponibles
        while ( pagina != NULL){
            // si el registro más viejo es la página siguiente, enlazo en la lista
            if(registroMasViejo == pagina->siguienteRegistroPagina){
                *(pagina)->siguienteRegistroPagina = *(registroMasViejo)->siguienteRegistroPagina;
                return registroMasViejo->registro.punteroAPagina;
            }
            pagina = pagina->siguienteRegistroPagina;
        }

        _tablaDeSegmentos = _tablaDeSegmentos->siguiente;
    }

}

// obtiene el registro más viejo y reenlaza la lista
registo_tad* liberarPagina() {

    tablaDePaginas* registroMasViejo = obtenerRegistroMasViejo();
    tablaDePaginas* aux = registroMasViejo;
    free(registroMasViejo);

    return reenlazarRegistros(aux);

}

/*** --------------- INSERT --------------- ***/






/*
reg_segmento* obtenerRegistroDeSegmento(char* nombreTabla){
    reg_segmento* _tablaDeSegmentos = tablaDeSegmentos;
    reg_segmento* _ultimoRegistroDeSegmento = NULL;

    while(_tablaDeSegmentos != NULL) {

        _ultimoRegistroDeSegmento = _tablaDeSegmentos;

        // valido si existe la tabla, en caso de existir, la retorno

        if (validarNombreTabla(nombreTabla, _tablaDeSegmentos->nombreTabla)){
            return _tablaDeSegmentos;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguienteRegistroSegmento;
    }

    // en caso de no existir, o no haber registro de segmentos, lo agrego

    return agregarRegistroDeSegmento(nombreTabla, _ultimoRegistroDeSegmento);
}


// Agrega un registro de Segmento. En caso de ser el primero, asigna al puntero tablaDeSegmentos el primer registro creado

reg_segmento* agregarRegistroDeSegmento(char* nombreTabla, reg_segmento* ultimoRegistroDeSegmento){

    //creo el nuevo registro
    reg_segmento* nuevoRegistroDeSegmento = malloc(sizeof(reg_segmento));
    nuevoRegistroDeSegmento->nombreTabla = nombreTabla;
    nuevoRegistroDeSegmento->siguienteRegistroSegmento = NULL;

    // no hay registros de segmento
    if(ultimoRegistroDeSegmento == NULL){
        tablaDeSegmentos = nuevoRegistroDeSegmento;
    } else {
        //hay registros de segmento, lo agrego al final
        ultimoRegistroDeSegmento->siguienteRegistroSegmento = nuevoRegistroDeSegmento;
    }

    return nuevoRegistroDeSegmento;
}


// Inicializa la tabla de segmentos



bool validarNombreTabla(char* nombreBuscado, char* nombreTabla){
    return strcmp(nombreBuscado, nombreTabla) == 0;
}

bool validarExistenciaDeSegmento(char* nombreTabla){
    reg_segmento* _tablaDeSegmentos = tablaDeSegmentos;
    while(_tablaDeSegmentos != NULL) {
        if (validarNombreTabla(nombreTabla, _tablaDeSegmentos->nombreTabla)){
            return true;
        }
        _tablaDeSegmentos = _tablaDeSegmentos->siguienteRegistroSegmento;
    }
    return false;
}*/

void recibir_valores_FileSystem(uint32_t servidorFileSystem) {
    tamanoValue = deserializar_int(servidorFileSystem);
    tiempoDump = deserializar_int(servidorFileSystem);
}

reg_marco* crearTablaDeMarcos(){
    cantDeMarcos = config.TAM_MEM/sizeof(registo_tad);
    reg_marco* aux = calloc(cantDeMarcos, sizeof(reg_marco));
    log_info(log_Memoria, "Se ha creado la tabla de marcos");
}

registo_tad* alocar_MemoriaPrincipal() {
    tablaDeMarcos = crearTablaDeMarcos();
    registo_tad* aux = malloc(config.TAM_MEM);
    log_info(log_Memoria, "Se ha alocado la memoria principal");

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

		//TODO Recibir tamano_value y tiempo_dump de File System

		//lo necesitamos para crear el espacio de la memoria (tamano_value) y el tiempo de dump

		// acá va la función recibir_valores_fileSystem, por ahora se hardcodean los valores

        tamanoValue = 16;
        tiempoDump = 4;

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

		//TODO aca se reciben los comandos de lo que se conecte a la memoria

        case NUEVA_CONEXION: {
            log_info(log_Memoria, "Se realizo una nueva conexion");
            break;
        }
        case COMAND_INSERT: {
            log_info(log_Memoria, "Insert");
            break;
        }
        case COMAND_SELECT: {
            log_info(log_Memoria, "El kernel envio un select");
            break;
        }
        case COMAND_CREATE: {
            log_info(log_Memoria, "El kernel envio un create");
            break;
        }
        case COMAND_DESCRIBE: {
            log_info(log_Memoria, "El kernel envio un describe");
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