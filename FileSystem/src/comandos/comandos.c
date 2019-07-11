//
// Created by utnso on 08/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_FileSystem, message);
    printf("%s", message);
}

void comando_insert(char* table, int key, char* value, uint64_t timestamp, int requestOrigin){
    log_info(log_FileSystem, "REQUEST INSERT ==> TABLA: <%s>\t KEY: <%d>\t VALUE: <%s>", table, key, value);
    string_to_upper(table);
    char* tabla_objetivo = string_duplicate(montajeTablas);
    string_append(&tabla_objetivo, table);

    uint64_t currentTime = timestamp;

    // NOT_TIMESTAMP = NULL
    if(timestamp == NOT_TIMESTAMP) {
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

    log_info(log_FileSystem, "SUCCESS INSERT ==> TABLA: <%s>\t VALUE: <%s>\t KEY <%d>", table, value, key);

}

void comando_create(char* table, char* consistencia, char* cantidad_particiones, char* compactacion, int requestOrigin) {
    log_info(log_FileSystem, "EXECUTE CREATE ==> TABLA: <%s>", table);

    int particiones = atoi(cantidad_particiones);

    string_to_upper(table);

    //consistencia = string_duplicate(string_itoa(fabs(atoi(consistencia))));
    string_to_upper(consistencia);

    //cantidad_particiones = string_duplicate(string_itoa(fabs(atoi(cantidad_particiones))));
    //string_to_upper(cantidad_particiones);

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

        createThreadCompactation(table, consistencia, atoi(cantidad_particiones), atoi(compactacion));
        log_info(log_FileSystem, "SUCCESS CREATE ==> TABLA <%s>", table);

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, CREATE_OK);
        }

    }
}

void comando_select(char* table, int key, int requestOrigin){

    lock_read_table(table);

    log_info(log_FileSystem, "REQUEST SELECT ==> TABLE: <%s>\t KEY: <%d>", table, key);

    registro_tad* finalResult = NULL;

    char* tabla_objetivo = string_duplicate(montajeTablas);
    string_append(&tabla_objetivo, table);

    int existe = ValidarArchivo(tabla_objetivo);

    if( existe != true ) {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, NO_EXISTE_TABLA);
        }

        log_info(log_FileSystem, "FAILED SELECT ==> LA TABLA <%s> NO EXISTE", table);

        free(finalResult);
        free(tabla_objetivo);

        return;
    }

    string_append(&tabla_objetivo, "/Metadata.bin");

    t_config* metadata = obtener_metadata_table(tabla_objetivo);


    registro_tad* registerFromMemtable = getValueFromMemtable(table, key);
    if(registerFromMemtable != NULL) {
        finalResult = registerFromMemtable;
    }

    registro_tad* registerFromTemporal = getValueFromTemporalFile(table, key, ".tmp");
    finalResult = verifyMaxValue(finalResult, registerFromTemporal);

    registro_tad* registerFromTemporalC = getValueFromTemporalFile(table, key, ".tmpc");
    finalResult = verifyMaxValue(finalResult, registerFromTemporalC);

    uint32_t particion = key % config_get_int_value(metadata, "PARTITIONS");
    registro_tad* registerFromPartition = getValueFromPartition(table, key, ".bin", particion);
    finalResult = verifyMaxValue(finalResult, registerFromPartition);

    if(finalResult == NULL) {
        log_info(log_FileSystem, "FAILED SELECT ==> NO SE ENCONTRO NINGUN REGISTRO CON LA KEY <%d> EN LA TABLA <%s>", key, table);

        if( requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, false);
        }

    } else {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, true);
            serializar_registro(requestOrigin, finalResult);
        }

        log_info(log_FileSystem, "SUCCES SELECT ==> TABLA: <%s>\tkey: <%d>\tvalue: <%s>", table, key, finalResult->value);
    }

    unlock_rw_table(table);
}

void comando_describe_all(int requestOrigin){

    log_info(log_FileSystem, "EXECUTE DESCRIBE ALL");

    mostrar_metadatas(requestOrigin);

}

void comando_describe(char* nombre_tabla, int requestOrigin){
    lock_read_table(nombre_tabla);

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

    } else {

        if(requestOrigin != CONSOLE_REQUEST) {
            serializar_int(requestOrigin, NO_EXISTE_TABLA);
        }

        log_info(log_FileSystem, "FAILED DESCRIBE ==> TABLA <%s>", nombre_tabla);
    }

    unlock_rw_table(nombre_tabla);
}

void comando_drop(char* table, int requestOrigin){

    lock_mx_drop(table);
    lock_write_table(table);

    log_info(log_FileSystem, "EXECUTE DROP");

    string_to_upper(table);

    char* tabla_objetivo = strdup(montajeTablas);
    string_append(&tabla_objetivo, table);

    int existe = ValidarArchivo(tabla_objetivo);

    if( existe == true ) {

        /*Libero la Memtable*/
        pthread_mutex_lock(&SEM_MEMTABLE);
        dictionary_remove(memtable,table);
        pthread_mutex_unlock(&SEM_MEMTABLE);

        /*Libero los bloques de los Tmps y Tmpcs*/
        freeBlocksFromTemps(tabla_objetivo, ".tmp");
        freeBlocksFromTemps(tabla_objetivo, ".tmpc");

        /*Libero los bloques del FS*/
        freeBlocksFromFS(tabla_objetivo);

        /*Elimino el directorio*/
        uint32_t directorioRemovido = remove_directory(tabla_objetivo);

        log_info(log_FileSystem, "SUCCESS DROP ==> La tabla <%s> se elimino correctamente ", table);
        if(requestOrigin != CONSOLE_REQUEST){
            serializar_int(socket, true);
        }

    }else{

        log_info(log_FileSystem, "FAILED DROP ==> La tabla <%s> no existe", table);
        if(requestOrigin != CONSOLE_REQUEST){
            serializar_int(socket, false);
        }
    }

    unlock_rw_table(table);
    unlock_mx_drop(table);

    /*Eliminamos la tabla de la estructura de compactacion*/
    pthread_mutex_lock(&SEM_MX_MAP_COMPACTACTION);
    dictionary_remove(TABLES_COMPACTATION, table);
    pthread_mutex_unlock(&SEM_MX_MAP_COMPACTACTION);
}

void comando_dump(){

    dictionary_iterator(memtable, (void *) _dumpearTabla);

    pthread_mutex_lock(&SEM_MEMTABLE);
    dictionary_clean(memtable);
    pthread_mutex_unlock(&SEM_MEMTABLE);

}

void comando_compactation(char* table) {

    runCompactation(table);
}