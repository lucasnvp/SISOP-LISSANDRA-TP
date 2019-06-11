//
// Created by utnso on 08/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_FileSystem, message);
    printf("%s", message);
}

char* comando_select(char* table, int key, int requestOrigin){
    log_info(log_FileSystem, "Comando select para tabla %s:", table);

    char* finalResult = string_new();

    char* tabla_objetivo = string_duplicate(montajeTablas);
    string_append(&tabla_objetivo, table);

    uint32_t existe = ValidarArchivo(tabla_objetivo);

    if( existe != true ) {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, NO_EXISTE_TABLA);
        } else {
            log_info(log_FileSystem, "Se intento buscar en una tabla no existente %s", table);
        }

        log_info(log_FileSystem, "No existe la tabla %s", table);
        return NULL;
    }

    string_append(&tabla_objetivo, "/Metadata.bin");
    t_config * metadata = obtener_metadata_table(tabla_objetivo);
    uint32_t particiones = config_get_int_value(metadata, "PARTITIONS");

    uint32_t particion = key % particiones;

    registro_tad* registerFromMemtable = getValueFromMemtable(table, key);

    // leerBloque("3");

    // registro_tad* registerFromTemporal = getValueFromTemporal(table, key);

    /*
     * char* valueFromTemporalC = getValueFromTemporalInCompression(table, key);
     *
     * char* valueFromBlock = getValueFromBlock(table, key)
     */

    if(registerFromMemtable == NULL) {
        return NULL;

    }else {

        if(requestOrigin != CONSOLE_REQUEST) {

            return registerFromMemtable->value;

        } else {
            log_info(log_FileSystem, "Valor obtenido %s", registerFromMemtable->value);
            return NULL;

        }
    }

}

void comando_insert(char* table, int key, char* value, int timestamp, int requestOrigin){

    string_to_upper(table);
    char* tabla_objetivo = string_duplicate(montajeTablas);
    string_append(&tabla_objetivo, table);

    int currentTime = timestamp;

    // NOT_TIMESTAMP
    if(timestamp < 0) {
        currentTime = getCurrentTime();
    }

    int existe = ValidarArchivo(tabla_objetivo);

    if( existe != true ) {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, NO_EXISTE_TABLA);
        } else {

            log_info(log_FileSystem, "Se intento insertar en la tabla '%s' no existente", table);
        }

        log_info(log_FileSystem, "Se intento insertar en la tabla '%s' no existente", table);

        return;
    }

    registro_tad * registroTad = new_registro_tad(currentTime, key, value);

    insertValue(table, registroTad);

    if(requestOrigin != CONSOLE_REQUEST){
        serializar_int(requestOrigin, INSERT_OK);
    } else {
        log_info(log_FileSystem, "Se realizo el insert correctamente en la tabla '%s'", table);
    };

}

void comando_create(char* table, char* consistencia, char* cantidad_particiones, char* compactacion, int requestOrigin) {
    log_info(log_FileSystem, "Se ejecuta el comando create para la tabla '%s'", table);

    int particiones = atoi(cantidad_particiones);

    string_to_upper(table);
    string_to_upper(consistencia);

    char* nueva_tabla = strdup(montajeTablas);
    string_append(&nueva_tabla, table);

    int existe = ValidarArchivo(nueva_tabla);

    if( existe == true ) {

        log_info(log_FileSystem, "Se intentó crear una carpeta ya existente con el nombre '%s'", table);

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, YA_EXISTE_TABLA);
        }

    } else {

        crear_carpeta(nueva_tabla);
        crear_metadata_table(nueva_tabla, consistencia, cantidad_particiones, compactacion);
        crear_particiones(nueva_tabla, particiones);
        log_info(log_FileSystem, "Se creo una carpeta a través del comando CREATE: ", table);

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, CREATE_OK);
        }<

        CANTIDAD_TABLAS++;
        log_info(log_FileSystem, "La cantidad total de tablas actual es: %d", CANTIDAD_TABLAS);
    }
}

void comando_describe_all(int requestOrigin){

    log_info(log_FileSystem, "Se ejecuta el comando DESCRIBE ALL para todas las tablas");

    mostrar_metadatas(requestOrigin);

}

void comando_describe(char* nombre_tabla, int requestOrigin){
    log_info(log_FileSystem, "Se ejecuta el comando DESCRIBE para todas las tablas");

    string_to_upper(nombre_tabla);

    char* tabla_objetivo = string_duplicate(montajeTablas);
    string_append(&tabla_objetivo, nombre_tabla);
    string_append(&tabla_objetivo, "/Metadata.bin");

    int existe = ValidarArchivo(tabla_objetivo);

    if( existe == true) {
        t_config * metadata = obtener_metadata_table(tabla_objetivo);

        if(requestOrigin != CONSOLE_REQUEST){

            describe_tad* describeTad = crearDescribe(metadata, nombre_tabla);

            serializar_int(requestOrigin, DESCRIBE_OK);
            serializar_describe(requestOrigin, describeTad);

            free_describe_tad(describeTad);

        }else{

            mostrar_metadata_tabla(metadata, nombre_tabla);
        }

        config_destroy(metadata);

        log_info(log_FileSystem, "Se ejecutó el comando DESCRIBE para la tabla: %s \n", nombre_tabla);

    } else {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, NO_EXISTE_TABLA);
        }

        log_info(log_FileSystem, "No existe una tabla con el nombre %s \n", nombre_tabla);
    }
}

void comando_drop(char* table, int requestOrigin){
    log_info(log_FileSystem, "Se ejecutó el comando DROP");

    string_to_upper(table);

    char* tabla_objetivo = strdup(montajeTablas);
    string_append(&tabla_objetivo, table);
    string_append(&tabla_objetivo, "/0.bin");

    int existe = ValidarArchivo(tabla_objetivo);

    // TODO: recorrer directorio de la tabla para liberar los bloques usados
    // TODO: usar la funcion para elmiminar un directorio entero de las commonsFunctions
    if( existe == true ) {

        borrar_particion(tabla_objetivo);

        if(requestOrigin != CONSOLE_REQUEST){

            //TODO Serializar msj por socket
        }else{

            log_info(log_FileSystem, "La tabla %s se elimino correctamente ", table);
        }

    }
}

void comando_dump(){
    dictionary_iterator(memtable, (void *) _dumpearTabla);
    dictionary_clean(memtable);
}