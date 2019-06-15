//
// Created by utnso on 08/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_FileSystem, message);
    printf("%s", message);
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
        }

        log_info(log_FileSystem, "FALLO INSERT ==> NO EXISTE LA TABLA <%s>", table);

        return;
    }

    registro_tad * registroTad = new_registro_tad(currentTime, key, value);

    insertValue(table, registroTad);

    if(requestOrigin != CONSOLE_REQUEST){
        serializar_int(requestOrigin, INSERT_OK);
    }

    log_info(log_FileSystem, "SUCCESS INSERT ==> TABLA <%s> , VALUE <%s>, KEY <%d>", table, value, key);

}

void comando_create(char* table, char* consistencia, char* cantidad_particiones, char* compactacion, int requestOrigin) {
    log_info(log_FileSystem, "EXECUTE CREATE ==> TABLA <%s>", table);

    int particiones = atoi(cantidad_particiones);

    string_to_upper(table);
    string_to_upper(consistencia);

    char* nueva_tabla = strdup(montajeTablas);
    string_append(&nueva_tabla, table);

    int existe = ValidarArchivo(nueva_tabla);

    if( existe == true ) {

        log_info(log_FileSystem, "FAIL CREATE ==> YA EXISTE UNA TABLA <%s>", table);

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, YA_EXISTE_TABLA);
        }

    } else {

        crear_carpeta(nueva_tabla);
        crear_metadata_table(nueva_tabla, consistencia, cantidad_particiones, compactacion);
        crear_particiones(nueva_tabla, particiones);
        log_info(log_FileSystem, "SUCCESS CREATE ==> TABLA <%s>", table);

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, CREATE_OK);
        }

        CANTIDAD_TABLAS++;
        log_info(log_FileSystem, "CANTIDAD DE TABLAS ACTUALES: %d", CANTIDAD_TABLAS);
    }
}

void comando_select(char* table, int key, int requestOrigin){
    log_info(log_FileSystem, "SELECT ==> TABLE <%s> , KEY<%d>", table, key);

    char* finalResult = string_new();

    char* tabla_objetivo = string_duplicate(montajeTablas);
    string_append(&tabla_objetivo, table);

    int existe = ValidarArchivo(tabla_objetivo);

    if( existe != true ) {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, NO_EXISTE_TABLA);
        }

        log_info(log_FileSystem, "FALLO SELECT ==> LA TABLA <%s> NO EXISTE", table);

        free(finalResult);
        free(tabla_objetivo);

        return;
    }

    string_append(&tabla_objetivo, "/Metadata.bin");

    // Obtenemos value de la metadata
    t_config* metadata = obtener_metadata_table(tabla_objetivo);
    uint32_t particiones = config_get_int_value(metadata, "PARTITIONS");

    registro_tad* registerFromMemtable = getValueFromMemtable(table, key);

    if(registerFromMemtable != NULL) {
        finalResult = string_duplicate(registerFromMemtable->value);
    }

    // Obtenemos value de los tmp
    registro_tad* registerFromTemporal = getValueFromTemporals(table, key);

    if(registerFromTemporal != NULL) {

        if(registerFromMemtable != NULL) {
            if(registerFromTemporal->timestamp > registerFromMemtable->timestamp) {
                finalResult = string_duplicate(registerFromTemporal->value);
            } else {
                finalResult = string_duplicate(registerFromMemtable->value);
            }
        } else {
            finalResult = string_duplicate(registerFromTemporal->value);
        }
    }

    // Obtenemos value de las particiones
    uint32_t particion = key % particiones;
    if(string_is_empty(finalResult)) {
        log_info(log_FileSystem, "FAILED SELECT ==> NO SE ENCONTRO NINGUN REGISTRO CON ESTA KEY <%d> ", key);

        if( requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, false);
        }

    }else {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, true);
            serializar_string(requestOrigin, finalResult);
        }

        log_info(log_FileSystem, "SELECT => TABLA: <%s>\tkey: <%d>\tvalue: <%s>", table, key, finalResult);
    }

    free(finalResult);
}

void comando_describe_all(int requestOrigin){

    log_info(log_FileSystem, "EXECUTE DESCRIBE ALL");

    mostrar_metadatas(requestOrigin);

}

void comando_describe(char* nombre_tabla, int requestOrigin){
    log_info(log_FileSystem, "EXECUTE DESCRIBE ==> TABLA <%s>", nombre_tabla);

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

        log_info(log_FileSystem, "SUCCES DESCRIBE ==> TABLA <%s>", nombre_tabla);

    } else {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, NO_EXISTE_TABLA);
        }

        log_info(log_FileSystem, "FAILED DESCRIBE ==> TABLA <%s>", nombre_tabla);
    }
}

void comando_drop(char* table, int requestOrigin){
    log_info(log_FileSystem, "EXECUTE DROP");

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