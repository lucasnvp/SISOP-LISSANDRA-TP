#include "compactator.h"

compactation_table_tad* new_compactation_table_tad(create_tad* tableInfo) {
    compactation_table_tad* compactationTable = malloc(sizeof(compactation_table_tad));
    compactationTable->tableInfo = tableInfo;
    return compactationTable;
}

void free_compactation_table_tad(compactation_table_tad* compactationTable) {
    free_create_tad(compactationTable->tableInfo);
    free(compactationTable);
}

void init_list_compactation() {
    TABLES_COMPACTATION = dictionary_create();
}

void runCompactation(char* table) {

    t_list* registers = list_create();

    char* path = string_duplicate(montajeTablas);
    string_append(&path, table);

    uint32_t partition = 0;
    char* pathTemporal = string_duplicate(path);
    string_append(&pathTemporal, "/");
    string_append(&pathTemporal, string_itoa(partition+1));
    string_append(&pathTemporal, ".tmpc");

    if(ValidarArchivo(path) != true) {
        log_info(log_FileSystem, "NO EXISTE LA TABLA %s", table);
        return;
    }

    string_append(&path, "/");
    bool ok = tmpToTmpc(path);

    if(ok) {
        list_add_all(registers, getRegistersFromTemporals(table, ".tmpc"));
    } else {
        return;
    }

    bool mientrasExistanParticiones = true;

    while (mientrasExistanParticiones == true) {

        char* pathPartition = string_duplicate(path);
        string_append(&pathPartition, string_itoa(partition));

        string_append(&pathPartition, ".bin");

        if(ValidarArchivo(pathPartition) == true) {

            t_list* listFromPartition = getRegistersFromPartition(table, ".bin", partition);

            char* pathForFilter = string_duplicate(montajeTablas);
            string_append(&pathForFilter, table);
            string_append(&pathForFilter, "/Metadata.bin");
            t_config* metadata = obtener_metadata_table(pathForFilter);

            bool _filterParticion(registro_tad* registro) {
                return (registro->key % config_get_int_value(metadata, "PARTITIONS")) == partition;
            }

            t_list* listaFiltrada = list_filter(registers, (void*) _filterParticion);

            for(int i = 0; i < list_size(listaFiltrada); i++) {

                registro_tad* registroTad = list_get(listaFiltrada, i);

                bool _mismaKey(registro_tad* registro) {
                    return registro->key == registroTad->key;
                }

                if(listFromPartition != NULL) {

                    registro_tad* registroTadFind = list_find(listFromPartition, (void*) _mismaKey);

                    if(registroTadFind != NULL) {

                        if(registroTad->timestamp > registroTadFind->timestamp) {
                            list_remove_by_condition(listFromPartition, (void*) _mismaKey);
                            list_add(listFromPartition, registroTad);
                        }

                    } else {

                        list_add(listFromPartition, registroTad);
                    }

                    free_registro_tad(registroTadFind);

                } else {

                    listFromPartition = list_create();
                    list_add(listFromPartition, registroTad);
                }

            }

            if(listFromPartition != NULL) {

                liberarBloques(pathPartition);
                remove(pathPartition);

                t_list* bloquesParaAsignar = list_create();

                char* registrosAGuardar = transformAllRegistersToUniqueString(listFromPartition);

                int hayLugar = getBloquesNecesariosParaEscribirRegistros(registrosAGuardar, bloquesParaAsignar);

                if(hayLugar == true) {
                    crearParticionCompactada(pathPartition, bloquesParaAsignar, string_length(registrosAGuardar));
                    guardarEnBloques(registrosAGuardar, bloquesParaAsignar);
                } else {
                    log_info(log_FileSystem, "FILE SYSTEM FULL ==> No hay la cantidad de bloques libres necesarios para realizar la transaccion en este momento");
                }

                list_destroy(bloquesParaAsignar);
                free(registrosAGuardar);
            }

            if(listFromPartition != NULL) {
                list_destroy(listFromPartition);
            }

            if(listaFiltrada != NULL) {
                list_destroy(listaFiltrada);
            }

            free(pathForFilter);

        } else {
            mientrasExistanParticiones = false;
        }

        partition++;

    }

    eliminarTmpc(path);

    free(path);
}

void eliminarTmpc(char* path) {

    uint32_t tmp = 1;
    uint32_t mientrasExistanArchivos = true;

    while(mientrasExistanArchivos == true) {

        char* pathTmp = string_duplicate(path);
        string_append(&pathTmp, string_itoa(tmp));
        string_append(&pathTmp, ".tmpc");

        if(ValidarArchivo(pathTmp) == true) {

            liberarBloques(pathTmp);
            remove(pathTmp);

        } else {

            mientrasExistanArchivos = false;

        }

        tmp++;

        free(pathTmp);
    }
}

bool tmpToTmpc(char* path) {

    uint32_t mientrasExistanArchivos = true;
    uint32_t tmp = 1;
    bool ok = false;

    while(mientrasExistanArchivos == true) {

        char* pathTmp = string_duplicate(path);
        string_append(&pathTmp, string_itoa(tmp));
        string_append(&pathTmp, ".tmp");

        char* pathTmpC = string_duplicate(path);
        string_append(&pathTmpC, string_itoa(tmp));
        string_append(&pathTmpC, ".tmpc");

        if(ValidarArchivo(pathTmp) == true) {

            rename(pathTmp, pathTmpC);
            ok = true;

        } else {

            mientrasExistanArchivos = false;

        }

        tmp++;

        free(pathTmp);
        free(pathTmpC);
    }

    return ok;

}

void liberarBloques(char* path) {

    char** bloquestodelete = get_bloques_array(path);

    for(i=0 ; bloquestodelete[i] != NULL ; i++){
        bitarray_clean_bit(bitarray, atoi(bloquestodelete[i]));
    }

    free(bloquestodelete);

}

void crearParticionCompactada(char* path, t_list* bloques, int size) {

    if(ValidarArchivo(path) == false) {

        FILE * newFD;
        newFD = fopen(path, "w+");

        char* tamanioDeBloques = string_new();
        string_append(&tamanioDeBloques, "SIZE=");
        string_append(&tamanioDeBloques, string_itoa(size));
        string_append(&tamanioDeBloques, "\n");
        fwrite(tamanioDeBloques,1,strlen(tamanioDeBloques),newFD);

        char* bloquesDelArchivo = string_new();
        string_append(&bloquesDelArchivo, "BLOCKS=[");

        for(int i = 0; i< list_size(bloques); i++) {
            int bloque = (int)list_get(bloques, i);
            string_append(&bloquesDelArchivo, string_itoa(bloque));

            int pos = i;
            if(pos+1 < list_size(bloques)) {
                string_append(&bloquesDelArchivo, ",");
            }
        }

        string_append(&bloquesDelArchivo, "]\n");
        fwrite(bloquesDelArchivo,1,strlen(bloquesDelArchivo),newFD);

        free(bloquesDelArchivo);
        fclose(newFD);

    }

}

void createThreadCompactation(char* nameTable, char* consistencia, u_int32_t particiones, u_int32_t compactacion) {
    create_tad* tableInfo = new_create_tad(nameTable, consistencia, particiones, compactacion);
    compactation_table_tad* compactationTable = new_compactation_table_tad(tableInfo);

    char* toSend = string_duplicate(nameTable);

    dictionary_put(TABLES_COMPACTATION, nameTable, compactationTable);
    pthread_create(&thread_compactation, NULL, (void*) execCompactation, (void*)toSend);

}

void execCompactation(void *param) {
    char* nameTable = param;

    compactation_table_tad* compactationTable = dictionary_get(TABLES_COMPACTATION, nameTable);

    bool forEverOrKillHim = true;
    while(forEverOrKillHim) {

        if(dictionary_has_key(TABLES_COMPACTATION, nameTable) == true) {
            usleep(compactationTable->tableInfo->compactacion*1000);

            // Una vez pasado el tiempo, compactamos
            runCompactation(compactationTable->tableInfo->nameTable);
            log_info(log_FileSystem, "Se compacta la tabla: %s", compactationTable->tableInfo->nameTable);
        } else {
            forEverOrKillHim = false;
        }

    }

    pthread_exit(NULL);
}
