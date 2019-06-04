//
// Created by utnso on 02/06/19.
//

#include "getterValuesFromFS.h"

registro_tad* getValueFromTemporal(char* table, int key) {

    int mientrasExistanTemporales = true;
    int tmp = 1;
    registro_tad* registroMax = NULL;

    while(mientrasExistanTemporales == true) {

        char* pathBase = string_duplicate(table);
        string_append(&pathBase, string_itoa(tmp));
        string_append(&pathBase, ".tmp");

        if(ValidarArchivo(pathBase) == true) {

            // TODO: leer valores y transformarlo en una lista de registros

        } else {

            mientrasExistanTemporales = false;
        }

        tmp++;
        free(pathBase);

    }

    return registroMax;
}
