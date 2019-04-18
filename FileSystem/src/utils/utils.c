
#include "utils.h"

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
