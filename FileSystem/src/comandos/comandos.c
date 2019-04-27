//
// Created by utnso on 08/04/19.
//

#include "comandos.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_FileSystem, message);
    printf("%s", message);
}

void comando_select(){
    print_console((void*) log_info, "Comando select \n");
}

void comando_insert(char* table, int key, char* value, int timestamp){
    print_console((void*) log_info, "Comando insert \n");

    int existe = ValidarArchivo(table);

    if( existe != true ) {
        log_info(log_FileSystem, "Se intento insertar en una tabla no existente %s", table);
        printf("Se intento insertar en una tabla no existente %s", table);
        return;
    }

    registro_tad * registroTad = new_registro_tad(timestamp, key, value);

    t_list * list;

    if(!dictionary_has_key(memtable, table)) {
        list = list_create();
    } else {
        list = dictionary_get(memtable, table);
    }

    list_add(list, registroTad);
    dictionary_put(memtable, table, list);
}

void comando_create(char* table, char* consistencia, char* cantidad_particiones, char* compactacion) {
    print_console((void*) log_info, "Se ejecuta el comando create\n");

    int particiones = atoi(cantidad_particiones);

    string_to_upper(table);
    string_to_upper(consistencia);

    char* nueva_tabla = strdup(montajeTablas);
    string_append(&nueva_tabla, table);

    int existe = ValidarArchivo(nueva_tabla);

    if( existe == true ) {
        log_info(log_FileSystem, "Se intentó crear una carpeta ya existente con el nombre %s", table);
        printf("Se intentó crear una carpeta ya existente con el nombre %s", table);
        // TODO: retornar error, validar con los demás
    } else {
        crear_carpeta(nueva_tabla);
        crear_metadata_table(nueva_tabla, consistencia, cantidad_particiones, compactacion);
        crear_particiones(nueva_tabla, particiones);
        log_info(log_FileSystem, "Se creo una carpeta a través del comando CREATE: ", table);
        printf("Se creo una carpeta a través del comando CREATE: %s \n", table);

        cantidad_bloquesLibres();
        CANTIDAD_TABLAS++;
        log_info(log_FileSystem, "La cantidad total de tablas actual es: ", CANTIDAD_TABLAS);
        printf("La cantidad total de tablas actual es: %i \n", CANTIDAD_TABLAS);
    }
}

void comando_describe_all(){
    print_console((void*) log_info, "Comando describe \n");
    mostrar_metadatas();
    log_info(log_FileSystem, "Se ejecutó el comando DESCRIBE para todas las tablas");
}

void comando_describe(char* nombre_tabla){
    print_console((void*) log_info, "Comando describe \n");

    string_to_upper(nombre_tabla);

    char* tabla_objetivo = string_duplicate(montajeTablas);
    string_append(&tabla_objetivo, nombre_tabla);
    string_append(&tabla_objetivo, "/Metadata.bin");

    int existe = ValidarArchivo(tabla_objetivo);

    if( existe == true) {
        t_config * metadata = obtener_metadata_table(tabla_objetivo);

        mostrar_metadata_tabla(metadata, nombre_tabla);

        config_destroy(metadata);

        log_info(log_FileSystem, "Se ejecutó el comando DESCRIBE para la tabla: ", nombre_tabla);

    } else {
        log_info(log_FileSystem, "No existe una tabla con el nombre ", nombre_tabla);
        printf("NO EXISTE UNA TABLA CON EL NOMBRE: %s \n", nombre_tabla);
    }
}

void comando_drop(char* table){
    print_console((void*) log_info, "Comando drop \n");

    string_to_upper(table);

    char* tabla_objetivo = strdup(montajeTablas);
    string_append(&tabla_objetivo, table);
    string_append(&tabla_objetivo, "/0.bin");

    int existe = ValidarArchivo(tabla_objetivo);

    // TODO: recorrer directorio de la tabla para liberar los bloques usados
    // TODO: usar la funcion para elmiminar un directorio entero de las commonsFunctions
    if( existe == true ) {
        borrar_particion(tabla_objetivo);
    }
}
