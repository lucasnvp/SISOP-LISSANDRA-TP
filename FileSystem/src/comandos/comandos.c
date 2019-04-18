//
// Created by utnso on 08/04/19.
//

#include "comandos.h"
#include "../setup/setup.h"
#include "../utils/utils.h"

void print_console(void (*log_function)(t_log*, const char*), char* message) {
    log_function(log_FileSystem, message);
    printf("%s", message);
}

void comando_select(){
    print_console((void*) log_info, "Comando select");
}

void comando_insert(){
    print_console((void*) log_info, "Comando insert");
}

void comando_create(){
    print_console((void*) log_info, "Se ejecuta el comando create\n");

    // TODO: reemplazar parámetros de prueba por lo que llegan en el request
    char* table = "gabe";
    string_to_upper(&table);

    char* nueva_tabla = strdup(montajeTablas);
    string_append(&nueva_tabla, table);

    int existe = ValidarArchivo(nueva_tabla);

    if( existe == 1 ) {
        puts("YA EXISTE UNA CARPETA CON ESTE NOMBRE");
        log_info(log_FileSystem, "Se intentó crear una carpeta ya existente con el nombre %s", table);
        // TODO: retornar error, validar con los demás
    } else {
        crear_carpeta(nueva_tabla);
        crear_metadata_table(nueva_tabla);
        // TODO: asignar un bloque a cada bin (particion)
    }
}

void comando_describe(){
    print_console((void*) log_info, "Comando describe");
}

void comando_drop(){
    print_console((void*) log_info, "Comando drop");
}
