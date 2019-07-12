
#include "utils.h"

void crear_carpeta(char* path) {
    mkdir(path, 0777);
}

void inicilizar_memtable() {
    memtable = dictionary_create();
}

void crear_metadata_table(char* nueva_tabla, char* consistencia, char* particiones, char* compactacion) {
    char* metadatabin = string_new();
    string_append(&metadatabin, nueva_tabla);
    string_append(&metadatabin, "/Metadata.bin");
    new_metadata_table(metadatabin, consistencia, particiones, compactacion);
    free(metadatabin);
}

/*char* settear_path_tabla(char* nombre_tabla) {

    string_to_upper(&nombre_tabla);

    char* nueva_tabla = strdup(montajeTablas);
    string_append(&nueva_tabla, nombre_tabla);

    return nueva_tabla;
}*/

void new_metadata_table(char* metadatabin, char* consistencia, char* particiones, char* compactacion){
    char* _consistencia = string_new();
    string_append(&_consistencia, "CONSISTENCY=");
    string_append(&_consistencia, consistencia);
    string_append(&_consistencia, "\n");

    char* _particiones = string_new();
    string_append(&_particiones, "PARTITIONS=");
    string_append(&_particiones, particiones);
    string_append(&_particiones, "\n");

    char* _compactacion = string_new();
    string_append(&_compactacion, "COMPACTATION_TIME=");
    string_append(&_compactacion, compactacion);
    string_append(&_compactacion, "\n");

    FILE * metadata_table = fopen(metadatabin, "w+b");
    fwrite(_consistencia,1,strlen(_consistencia),metadata_table);
    fwrite(_particiones,1,strlen(_particiones),metadata_table);
    fwrite(_compactacion,1,strlen(_compactacion),metadata_table);
    fclose(metadata_table);
    free(_consistencia);
    free(_particiones);
    free(_compactacion);
}

t_config * obtener_metadata_table(char* metadatabin){
    return config_create(metadatabin);
}

void mostrar_metadata_tabla(t_config * metadata, char* nombre_tabla) {
    log_info(log_FileSystem,"--------------------------------------");
    log_info(log_FileSystem,"Metadata de la tabla: %s \n", nombre_tabla);
    log_info(log_FileSystem,"CONSISTENCY: %s \n", config_get_string_value(metadata, "CONSISTENCY"));
    log_info(log_FileSystem,"PARTITIONS: %i \n", config_get_int_value(metadata, "PARTITIONS"));
    log_info(log_FileSystem,"COMPACTATION_TIME: %i \n", config_get_int_value(metadata, "COMPACTATION_TIME"));
    log_info(log_FileSystem,"--------------------------------------");
}

describe_tad* crearDescribe(t_config* metadata, char* nombreTabla) {


    return new_describe_tad(nombreTabla,
            config_get_string_value(metadata, "CONSISTENCY"),
            config_get_int_value(metadata, "PARTITIONS"),
            config_get_int_value(metadata, "COMPACTATION_TIME"));
}

void mostrar_metadatas(int requestOrigin) {

    DIR *d = opendir(montajeTablas);

    t_list* describes = list_create();

    bool existeAlMenosUnaTabla = false;

    if (d) {
        struct dirent *p;

        while (p=readdir(d)) {

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            lock_read_table(p->d_name);
            char* path = string_duplicate(montajeTablas);
            string_append(&path, p->d_name);
            string_append(&path, "/Metadata.bin");

            t_config * metadata = obtener_metadata_table(path);

            if(requestOrigin != CONSOLE_REQUEST) {
                list_add(describes, crearDescribe(metadata, p->d_name));
            } else {
                existeAlMenosUnaTabla = true;
            }

            mostrar_metadata_tabla(metadata, p->d_name);

            free(path);
            config_destroy(metadata);

            unlock_rw_table(p->d_name);
        }

        closedir(d);
    }

    if(requestOrigin != CONSOLE_REQUEST) {
        if(list_is_empty(describes) == true) {
        } else {
            serializar_describe_all(requestOrigin, describes);
        };
    } else {
        if(existeAlMenosUnaTabla == false) {
            log_info(log_FileSystem, "NOT DESCRIBE ==> No existen tablas en el directorio");
        }
    }

    list_destroy(describes);

}

void crear_particiones(char* path, int cantidad_particiones) {

    for(int i = 0; i < cantidad_particiones; i++) {
        char* particion = string_duplicate(crear_path_particion(path, i));
        asignar_bloques(particion);
        free(particion);
    }
}

char* crear_path_particion(char* path, int numero_particion) {
    char* particion = string_new();
    string_append(&particion, path);
    string_append(&particion, "/");
    string_append(&particion, string_itoa(numero_particion));
    string_append(&particion, ".bin");

    return particion;
}

void asignar_bloques(char* path) {

    if(ValidarArchivo(path) == false){
        FILE * newFD;
        newFD = fopen(path, "w+");

        char* tamanioDeBloques = string_new();
        string_append(&tamanioDeBloques, "SIZE=0\n");
        fwrite(tamanioDeBloques,1,strlen(tamanioDeBloques),newFD);
        free(tamanioDeBloques);

        //Agregar el bloque
        int bloque_to_add = bloque_libre();

        if(bloque_to_add != -1){
            char* bloquesDelArchivo = string_new();
            string_append(&bloquesDelArchivo, "BLOCKS=[");
            string_append(&bloquesDelArchivo, string_itoa(bloque_to_add));
            string_append(&bloquesDelArchivo, "]\n");
            fwrite(bloquesDelArchivo,1,strlen(bloquesDelArchivo),newFD);

            bitarray_set_bit(bitarray, bloque_to_add);
            free(bloquesDelArchivo);
        } else{
            // TODO: que hacer si no hay bloques libres? Deberia permitirte crear la tabla igual (como esta ahora?) o debería retornar error?
            log_info(log_FileSystem, "FILE SYSTEM FULL ==> No hay la cantidad de bloques libres necesarios para realizar la transaccion en este momento");
        }
        fclose(newFD);
    }
}

int bloque_libre() {
    for(i = 0; i < bitarray_get_max_bit(bitarray); i++){
        if(bitarray_test_bit(bitarray, i) == false){
            return i;
        }
    }
    //No hay bloques
    return -1;
}

int cantidad_bloquesLibres(){

    int bloques_libres = 0;
    int bloque_libre;
    int bit = 0;
    int tamMaximo = CANT_BLOQUES;
    while(bit < tamMaximo)
    {
        bloque_libre = bitarray_test_bit(bitarray,bit);
        if(bloque_libre == 0)bloques_libres ++;
        bit++;
    }

    return bloques_libres;
}

void borrar_particion(char* path) {

    char** bloquestodelete = get_bloques_array(path);

    for(i=0 ; bloquestodelete[i] != NULL ; i++){
        bitarray_clean_bit(bitarray, atoi(bloquestodelete[i]));
    }
    free(bloquestodelete);

    if(remove(path) == 0){
        log_info(log_FileSystem, "Se elimino la tabla exitosamente");
    } else{
        log_info(log_FileSystem, "No se pudo eliminar la tabla");
    }
}

char** get_bloques_array(char* path) {
    t_config* filetogetbloques = config_create(path);
    char** bloquesarray = config_get_array_value(filetogetbloques, "BLOCKS");
    config_destroy(filetogetbloques);
    return bloquesarray;
}
