
#include "utils.h"
#include "../setup/setup.h"

void crear_carpeta(char* path) {
    mkdir(path, 0777);
}

void crear_metadata_table(char* nueva_tabla) {
    char* metadatabin = string_new();
    string_append(&metadatabin, nueva_tabla);
    string_append(&metadatabin, "/Metadata.bin");
    new_metadata_table(metadatabin);
    metadata_table_setup(metadatabin);
}

/*char* settear_path_tabla(char* nombre_tabla) {

    string_to_upper(&nombre_tabla);

    char* nueva_tabla = strdup(montajeTablas);
    string_append(&nueva_tabla, nombre_tabla);

    return nueva_tabla;
}*/

void new_metadata_table(char* metadatabin){
    FILE * metadata_table = fopen(metadatabin, "w+b");
    fwrite(CONSISTENCY,1,strlen(CONSISTENCY),metadata_table);
    fwrite(PARTITIONS,1,strlen(PARTITIONS),metadata_table);
    fwrite(COMPACTATION_TIME,1,strlen(COMPACTATION_TIME),metadata_table);
    fclose(metadata_table);
}

void metadata_table_setup(char* metadatabin){
    //Obtener Datos Metadata
    metadataTableConfig = config_create(metadatabin);
    CONSISTENCY = config_get_int_value(metadataTableConfig, "CONSISTENCY");
    PARTITIONS = config_get_int_value(metadataTableConfig, "PARTITIONS");
    COMPACTATION_TIME = config_get_int_value(metadataTableConfig, "COMPACTATION_TIME");
    config_destroy(metadataTableConfig);
}

void crear_particiones(char* path, int cantidad_particiones) {

    for(int i = 0; i < cantidad_particiones; i++) {
        char* particion = crear_path_particion(path, i);
        asignar_bloques(particion);
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

        // TODO: segun metadata del FS
        fwrite(TAMANIO_BLOQUE,1,strlen(TAMANIO_BLOQUE),newFD);

        //Agregar el bloque
        int32_t bloque_to_add = bloque_libre(bitarray);

        if(bloque_to_add != -1){
            char* bloquesDelArchivo = string_new();
            string_append(&bloquesDelArchivo, "BLOQUES=[");
            string_append(&bloquesDelArchivo, string_itoa(bloque_to_add));
            string_append(&bloquesDelArchivo, "]\n");
            fwrite(bloquesDelArchivo,1,strlen(bloquesDelArchivo),newFD);

            bitarray_set_bit(bitarray, bloque_to_add);
        } else{
            // TODO: que hacer si no hay bloques libres? Deberia permitirte crear la tabla igual (como esta ahora?) o debería retornar error
            //No hay bloques
        }
        fclose(newFD);
    }
}

int32_t bloque_libre(t_bitarray* auxBitArray) {
    bool testBit;
    for(i = 0; i <= bitarray_get_max_bit(auxBitArray); i++){
        if(bitarray_test_bit(auxBitArray, i) == false){
            return i;
        }
    }
    //No hay bloques
    return -1;
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
    char** bloquesarray = config_get_array_value(filetogetbloques, "BLOQUES");
    config_destroy(filetogetbloques);
    return bloquesarray;
}