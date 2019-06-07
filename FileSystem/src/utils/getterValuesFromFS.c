//
// Created by utnso on 02/06/19.
//

#include "getterValuesFromFS.h"

registro_tad* getValueFromTemporal(char* table, int key) {

    int mientrasExistanTemporales = true;
    int tmp = 1;
    registro_tad* registroMax = NULL;

    while(mientrasExistanTemporales == true) {

        char* pathBase = string_duplicate(montajeTablas);
        string_append(&pathBase,table);
        string_append(&pathBase,"/");
        string_append(&pathBase, string_itoa(tmp));
        string_append(&pathBase, ".tmp");

        if(ValidarArchivo(pathBase) == true) {

            /*
             * TODO: leer valores y transformarlo en una lista de registros
             * 1. Leer el archivo
             * 2. Tomar la lista de bloques en donde se encuentran los registros
             * 3.
             * */

            /*Tomo los bloques del TMP*/
            t_config * auxtmp;
            auxtmp = config_create(pathBase);
            char* bloques = config_get_string_value(auxtmp,"BLOQUES");

            /*Lo convierto a vector*/
            char** bloquesSrt = string_get_string_as_array(bloques);







        } else {

            mientrasExistanTemporales = false;
        }

        tmp++;
        free(pathBase);

    }


    return registroMax;

}