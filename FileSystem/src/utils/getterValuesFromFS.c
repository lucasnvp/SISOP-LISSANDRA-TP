//
// Created by utnso on 02/06/19.
//

#include "getterValuesFromFS.h"

registro_tad* getValueFromTemporals(char* table, int key) {

    uint32_t mientrasExistanTemporales = true;
    uint32_t bin = 0;
    uint32_t tmp = 1;
    registro_tad* registroMax = NULL;
    char* registrosConcatenados =  string_new();

    char* pathTabla = string_duplicate(montajeTablas);
    string_append(&pathTabla,table);
    string_append(&pathTabla,"/");

    char* pathTmp = string_duplicate(pathTabla);

    while(mientrasExistanTemporales == true) {

        string_append(&pathTmp, string_itoa(tmp));
        string_append(&pathTmp, ".tmp");

        if(ValidarArchivo(pathTmp) == true) {

            char* registrosConcatenadosDeUnTmp = string_new();

            registrosConcatenadosDeUnTmp =  getRegistrosConcatenadosDeUnTmp(pathTabla);

             string_append(&registrosConcatenados,registrosConcatenadosDeUnTmp);

            tmp++;

        } else {

            mientrasExistanTemporales = false;
        }

        free(pathTabla);

    }

    // TODO: hacer funcion para leer un bloque: leerBloque(nroBloque) -> char* lo que haya adentro

    return NULL;

}


char* getRegistrosConcatenadosDeUnTmp(char* pathTmp) {

    char* result = string_new();

    char* tmp = string_duplicate(pathTmp);

    string_append(&tmp, "/3.tmp");

    t_config * auxtmp = config_create(tmp);

    char* bloques = config_get_string_value(auxtmp, "BLOQUES");

    char** bloquesList = string_get_string_as_array(bloques);

    for (int i = 0; i < string_length(*bloquesList)-1; i++) {
        string_append(&result, leerBloque(bloquesList[i]));
    }

    string_iterate_lines(bloquesList, (void*) free);
    free(bloques);
    config_destroy(auxtmp);

    return result;

}

char* leerBloque(char* nroBloque) {

    char* path = string_new();
    path = string_duplicate(montajeBloques);
    char* nBloque = string_new();
    nBloque= string_duplicate(nroBloque);
    string_append(&path, nBloque);
    string_append(&path, ".bin");
    char* buffer = calloc(1, TAMANIO_BLOQUES);

    if(ValidarArchivo(path) == true) {
        FILE * newFD;
        newFD = fopen(path, "rb");

        fread(buffer, 1, TAMANIO_BLOQUES, newFD);

        fclose(newFD);
    }

    free(path);
    free(nBloque);

    return buffer;
}