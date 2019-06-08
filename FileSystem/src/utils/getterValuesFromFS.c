//
// Created by utnso on 02/06/19.
//

#include "getterValuesFromFS.h"

registro_tad* getValueFromTemporal(char* table, int key) {

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

char* getRegistrosConcatenadosDeUnTmp(char* pathTabla) {
    /*Tomo los bloques del TMP*/
    t_config * auxtmp;
    auxtmp = config_create(pathTabla);
    char* bloques = config_get_string_value(auxtmp,"BLOQUES");

    /*Lo convierto a vector*/
    char** bloquesStr = string_get_string_as_array(bloques);

    char* str = string_new();

    return str;

}