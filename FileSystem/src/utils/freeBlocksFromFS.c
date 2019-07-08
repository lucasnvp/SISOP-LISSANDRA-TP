//
// Created by utnso on 16/06/19.
//

#include <stdint.h>
#include "freeBlocksFromFS.h"
#include "../setup/setup.h"

void* freeBlocksFromTemps(char* path, char* typeFile){

    uint32_t mientrasExistanArchivos = true;
    uint32_t tmp = 1;

    while(mientrasExistanArchivos == true) {

        char* pathTmp = string_duplicate(path);
        string_append(&pathTmp,"/");
        string_append(&pathTmp, string_itoa(tmp));
        string_append(&pathTmp, typeFile);

        if(ValidarArchivo(pathTmp) == true) {

                       limpiarYLiberarBloque(pathTmp);
            tmp++;

        } else {

            mientrasExistanArchivos = false;

        }
    }
}


/*Lee el path del archivo, accede a los bloques. Los recorre, si los encuentra lo limpia y setea el bitarray como libre*/
void* limpiarYLiberarBloque(char* path){

    char* pathtmp = string_duplicate(path);

    char* pathBloques = string_duplicate(montajeBloques);

    t_config * auxtmp = config_create(pathtmp);

    char* bloques = config_get_string_value(auxtmp, "BLOCKS");

    char** bloquesList = string_get_string_as_array(bloques);

    for (uint32_t i = 0; bloquesList[i] != NULL; i++) {

        string_append(&pathBloques,bloquesList[i]);

        string_append(&pathBloques,".bin");

        if(ValidarArchivo(pathBloques)== true){

            bitarray_clean_bit(bitarray,atoi(bloquesList[i]));
                /*TODO: Probar crear 3 tablas y borrar la segunda.
                 * Crear una tercera con varias particiones y ver si asigna en los bloques de la segunda y los que siguen a la tercera*/
        }
    }

    free(pathBloques);
    free(bloques);
//    config_destroy(auxtmp);
    free(pathtmp);
    string_iterate_lines(bloquesList, (void*) free);

}


void* freeBlocksFromFS(char* path){

    uint32_t mientrasExistanArchivos = true;
    uint32_t bin = 0;

    while(mientrasExistanArchivos == true) {

        char* pathBin = string_duplicate(path);
        string_append(&pathBin,"/");
        string_append(&pathBin, string_itoa(bin));
        string_append(&pathBin, ".bin");

        if(ValidarArchivo(pathBin) == true) {

            limpiarYLiberarBloque(pathBin);
            bin++;

        } else {

            mientrasExistanArchivos = false;

        }
    }


}