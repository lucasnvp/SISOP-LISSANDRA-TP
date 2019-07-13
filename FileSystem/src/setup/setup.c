#include "setup.h"

void punto_montaje_setup(char* pdm){
    puntoDeMontaje =  strdup(pdm);

    montajeMetadata = string_new();
    string_append(&montajeMetadata, puntoDeMontaje);
    string_append(&montajeMetadata, "Metadata/");

    montajeTablas = string_new();
    string_append(&montajeTablas, puntoDeMontaje);
    string_append(&montajeTablas, "Tables/");

    montajeBloques = string_new();
    string_append(&montajeBloques, puntoDeMontaje);
    string_append(&montajeBloques, "Bloques/");

    setup();
}

void setup(){
    carpetas_setup();

    //Archivo Metadata.bin
    char* metadatabin = string_new();
    string_append(&metadatabin, montajeMetadata);
    string_append(&metadatabin, "Metadata.bin");

    if(access(metadatabin, F_OK) != -1){
        metadata_setup(metadatabin);
    } else{
        new_metadata_setup(metadatabin);
        metadata_setup(metadatabin);
    }

    //Archivo Bitmap.bin
    char* bitmapbin = string_new();
    string_append(&bitmapbin, montajeMetadata);
    string_append(&bitmapbin, "Bitmap.bin");

    if(access(bitmapbin, F_OK) != -1){
        bitmap_setup(bitmapbin);

    } else{
        //Bloques
        bloques_setup();
        new_bitmap_setup(bitmapbin);
        bitmap_setup(bitmapbin);
    }

    // Creamos los hilos para compactar por cada tabla existente
    setUp_compactation_threads();
}

void carpetas_setup(){
    //Crea la carpeta de montaje
    mkdir(puntoDeMontaje, 0777);
    //Crea la carpeta Metadata
    mkdir(montajeMetadata, 0777);
    //Crea la carpeta Tablas
    mkdir(montajeTablas, 0777);
    //Crea la carpeta Bloques
    mkdir(montajeBloques, 0777);
}

void metadata_setup(char* metadatabin){
    //Obtener Datos Metadata
    metadataConfig = config_create(metadatabin);
    CANT_BLOQUES = config_get_int_value(metadataConfig, "BLOCKS");
    TAMANIO_BLOQUES = config_get_int_value(metadataConfig, "BLOCK_SIZE");
    config_destroy(metadataConfig);
}

void new_metadata_setup(char* metadatabin){
    metadata = fopen(metadatabin, "w+b");

    char* blockSize = string_duplicate(TAMANIO_BLOQUE);
    string_append(&blockSize, string_itoa(config->BLOCK_SIZE));
    string_append(&blockSize, "\n");
    fwrite(blockSize,1,strlen(blockSize),metadata);
    free(blockSize);

    char* blocks = string_duplicate(CANTIDAD_BLOQUE);
    string_append(&blocks, string_itoa(config->BLOCKS));
    string_append(&blocks, "\n");
    fwrite(CANTIDAD_BLOQUE,1,strlen(CANTIDAD_BLOQUE),metadata);
    free(blocks);

    fwrite(MAGIC_NUMBER,1,strlen(MAGIC_NUMBER),metadata);
    fclose(metadata);
}

void bitmap_setup(char* bitmapbin){
    struct stat bitmapStat;
    int32_t fd = open(bitmapbin,O_RDWR);
    fstat(fd,&bitmapStat);
    mmapBitmap = mmap(NULL,bitmapStat.st_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
    bitarray = bitarray_create_with_mode(mmapBitmap,bitmapStat.st_size/8, LSB_FIRST);

    // Divido por 8 porque tengo que pasarle bits
}

void new_bitmap_setup(char* bitmapbin){
    bitmap = fopen(bitmapbin, "wb+");
    char* arrayLoco;
    arrayLoco = calloc(1, config->BLOCKS);
    fwrite(arrayLoco, 1, config->BLOCKS, bitmap);
    free(arrayLoco);
    fclose(bitmap);
}

void bloques_setup(){
    FILE * newBloque;
    for(i=0; i <= config->BLOCKS-1; i++){
        char* nroBloque = string_new();
        string_append(&nroBloque, montajeBloques);
        string_append(&nroBloque, string_itoa(i));
        string_append(&nroBloque, ".bin");
        newBloque = fopen(nroBloque,"w+b");
        fclose(newBloque);
        free(nroBloque);
    }
}

void setUp_compactation_threads(){

    DIR *d = opendir(montajeTablas);

    //TABLES_COMPACTATION = dictionary_create();
    //LIST_SEM_TABLES = dictionary_create();

    if (d) {
        struct dirent *p;

        while (p=readdir(d)) {

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            char* path = string_duplicate(montajeTablas);
            string_append(&path, p->d_name);
            string_append(&path, "/Metadata.bin");

            t_config * metadata = obtener_metadata_table(path);

            createThreadCompactation(p->d_name,
                    config_get_string_value(metadata, "CONSISTENCY"),
                    config_get_int_value(metadata, "PARTITIONS"),
                    config_get_int_value(metadata, "COMPACTATION_TIME"));

            free(path);
            config_destroy(metadata);
        }

        closedir(d);
    }

}