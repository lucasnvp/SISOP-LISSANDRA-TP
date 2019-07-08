//
// Created by utnso on 02/06/19.
//

#include "getterValuesFromFS.h"

registro_tad* getValueFromTemporalFile(char* table, int key, char* typeFile) {

    t_list* listaRegistros = getRegistersFromTemporals(table, typeFile);

    if(listaRegistros == NULL) {
        return NULL;
    }

    //registro_tad* registroTad = obtenerRegistroSegunKey(listaRegistros, key);
    return obtenerRegistroSegunKey(listaRegistros, key);

    // TODO: no me deja eliminar la lista, rompe
    //list_destroy(listaRegistros);

    //return registroTad;
}

t_list* getRegistersFromTemporals(char* table, char* typeFile) {

    uint32_t mientrasExistanArchivos = true;
    uint32_t tmp = 1;
    char* registrosConcatenados =  string_new();

    char* pathTabla = string_duplicate(montajeTablas);
    string_append(&pathTabla,table);
    string_append(&pathTabla,"/");


    while(mientrasExistanArchivos == true) {

        char* pathTmp = string_duplicate(pathTabla);
        string_append(&pathTmp, string_itoa(tmp));
        string_append(&pathTmp, typeFile);

        if(ValidarArchivo(pathTmp) == true) {

            char* registrosConcatenadosDeUnTmp = getRegistersFromBinaryFile(pathTmp);

            string_append(&registrosConcatenados,registrosConcatenadosDeUnTmp);

            tmp++;


        } else {

            mientrasExistanArchivos = false;

        }

        free(pathTmp);
    }

    free(pathTabla);

    if(string_is_empty(registrosConcatenados)) {
        return NULL;
    }

    return transformRegistersStrToStructs(registrosConcatenados);
}

t_list* getRegistersFromPartition(char* table, char* typeFile, uint32_t partition) {

    char* registrosConcatenados =  string_new();

    char* pathTabla = string_duplicate(montajeTablas);
    string_append(&pathTabla,table);
    string_append(&pathTabla,"/");
    string_append(&pathTabla, string_itoa(partition));
    string_append(&pathTabla, typeFile);

    if(ValidarArchivo(pathTabla) == true) {

        char* registrosConcatenadosDeUnBloque = getRegistersFromBinaryFile(pathTabla);

        if(registrosConcatenadosDeUnBloque == NULL) {
            return NULL;
        }

        string_append(&registrosConcatenados,registrosConcatenadosDeUnBloque);

        free(registrosConcatenadosDeUnBloque);

    } else {

        return NULL;

    }

    t_list* listaRegistros = transformRegistersStrToStructs(registrosConcatenados);

    if(listaRegistros == NULL) {
        return NULL;
    } else {
        return listaRegistros;
    }

}

registro_tad* getValueFromPartition(char* table, int key, char* typeFile, uint32_t partition) {

    t_list* listaRegistros = getRegistersFromPartition(table, typeFile, partition);

    if(listaRegistros == NULL) {
        return NULL;
    }

    return obtenerRegistroSegunKey(listaRegistros, key);
}


char* getRegistersFromBinaryFile(char *pathTmp) {

    char* result = string_new();

    char* tmp = string_duplicate(pathTmp);

    t_config * auxtmp = config_create(tmp);

    char* bloques = config_get_string_value(auxtmp, "BLOCKS");
    char* size = config_get_string_value(auxtmp, "SIZE");

    if(bloques != NULL && !string_equals_ignore_case(size, "0")) {

        char** bloquesList = string_get_string_as_array(bloques);

        // TODO: ir calculando cuanto queda por leer

        uint32_t int_size = atoi(size);
        for (int i = 0; i < bloquesList[i] != NULL; i++) {
            string_append(&result, leerBloque(bloquesList[i], int_size));
        }

        string_iterate_lines(bloquesList, (void*) free);
        free(bloques);

        // TODO SE ROMPE CUANDO QUIERO LIBERARLO
        //config_destroy(auxtmp);

        return result;
    } else {
        return NULL;
    }

}

char* leerBloque(char* nroBloque, int size) {

    char* path = string_new();
    path = string_duplicate(montajeBloques);
    char* nBloque = string_duplicate(nroBloque);
    string_append(&path, nBloque);
    string_append(&path, ".bin");
    char* buffer = calloc(1, size);

    if(ValidarArchivo(path) == true) {
        FILE * newFD;
        newFD = fopen(path, "rb");

        fread(buffer, 1, size, newFD);

        fclose(newFD);
    }

    free(path);
    free(nBloque);

    return buffer;
}

t_list* transformRegistersStrToStructs(char* strRegs) {

    char** registers = string_split(strRegs, "\n");

    t_list* list_registers = list_create();

    int i = 0;
    while(registers[i] != NULL) {

        if(string_contains(registers[i], ";")) {
            char* registro = registers[i];
            char** tad = string_split(registro, ";");

            list_add(list_registers, new_registro_tad(atoll(tad[0]), atoi(tad[1]), tad[2]));

            free(registro);
            string_iterate_lines(tad, (void*) free);
            free(tad);
        }


        i++;
    }

    // TODO: no me deja hacerle free, se rompe
    //string_iterate_lines(registers, (void*) free);
    free(registers);

    return list_registers;
}

registro_tad* obtenerRegistroSegunKey(t_list* registros, int key) {

    if(registros->elements_count == 0) {
        return NULL;
    }

    bool _mismaKey(registro_tad* registro) {
        return registro->key == key;
    }

    t_list * listaFiltrada = list_filter(registros, (void*) _mismaKey);

    list_destroy(registros);

    if(listaFiltrada->elements_count > 0) {
        bool _timestampMayor(registro_tad * primera, registro_tad * segunda) {
            return primera->timestamp > segunda->timestamp;
        }

        list_sort(listaFiltrada, (void*) _timestampMayor);

        return list_get(listaFiltrada,0);

    } else {

        //log_info(log_FileSystem, "No existe la key %d en los archivos temporales", key);
        return NULL;
    }
}

registro_tad* verifyMaxValue(registro_tad* finalResult, registro_tad *registerFrom) {
    if (registerFrom != NULL) {
        if (finalResult != NULL) {
            if (registerFrom->timestamp > finalResult->timestamp) {
                return registerFrom;
            }
        } else {
            return registerFrom;
        }
    }

    return finalResult;
}