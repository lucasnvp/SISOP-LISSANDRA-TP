//
// Created by utnso on 02/06/19.
//

#include "getterValuesFromFS.h"

registro_tad* getValueFromTemporals(char* table, int key) {

    uint32_t mientrasExistanTemporales = true;
    uint32_t tmp = 1;
    char* registrosConcatenados =  string_new();

    char* pathTabla = string_duplicate(montajeTablas);
    string_append(&pathTabla,table);
    string_append(&pathTabla,"/");


    while(mientrasExistanTemporales == true) {

        char* pathTmp = string_duplicate(pathTabla);
        string_append(&pathTmp, string_itoa(tmp));
        string_append(&pathTmp, ".tmp");

        if(ValidarArchivo(pathTmp) == true) {

            char* registrosConcatenadosDeUnTmp = getRegistrosConcatenadosDeUnTmp(pathTmp);

            string_append(&registrosConcatenados,registrosConcatenadosDeUnTmp);

            tmp++;

        } else {

            mientrasExistanTemporales = false;
        }

        free(pathTmp);
    }

    free(pathTabla);

    t_list* listaRegistros = transformRegistersStrToStructs(registrosConcatenados);

    //registro_tad* registroTad = obtenerRegistroSegunKey(listaRegistros, key);
    return obtenerRegistroSegunKey(listaRegistros, key);

    // TODO: no me deja eliminar la lista, rompe
    //list_destroy(listaRegistros);

    //return registroTad;
}


char* getRegistrosConcatenadosDeUnTmp(char* pathTmp) {

    char* result = string_new();

    char* tmp = string_duplicate(pathTmp);

    t_config * auxtmp = config_create(tmp);

    char* bloques = config_get_string_value(auxtmp, "BLOCKS");
    char* size = config_get_string_value(auxtmp, "SIZE");

    char** bloquesList = string_get_string_as_array(bloques);

    // TODO: ir calculando cuanto queda por leer
    for (int i = 0; i < string_length(*bloquesList); i++) {
        string_append(&result, leerBloque(bloquesList[i], atoi(size)));
    }

    string_iterate_lines(bloquesList, (void*) free);
    free(bloques);

    // TODO SE ROMPE CUANDO QUIERO LIBERARLO
    //config_destroy(auxtmp);

    return result;

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

        char* registro = registers[i];
        char** tad = string_split(registro, ";");

        list_add(list_registers, new_registro_tad(atoi(tad[0]), atoi(tad[1]), tad[2]));

        free(registro);
        string_iterate_lines(tad, (void*) free);
        free(tad);

        i++;
    }

    // TODO: no me deja hacerle free, se rompe
    //string_iterate_lines(registers, (void*) free);
    free(registers);

    return list_registers;
}

registro_tad* obtenerRegistroSegunKey(t_list* registros, int key) {

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

        log_info(log_FileSystem, "No existe la key %d en los archivos temporales", key);
        return NULL;
    }
}