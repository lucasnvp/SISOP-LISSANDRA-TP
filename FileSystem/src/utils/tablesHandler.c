
#include "tablesHandler.h"

void _crearTmp(char* nombreTabla, t_list * registros){

    t_list* bloquesParaAsignar = list_create();
    int hayLugar = getBloquesNecesarios(registros, bloquesParaAsignar);

    if(hayLugar) {
        crearArchivoTemporal(nombreTabla, bloquesParaAsignar);
        list_destroy(registros);
    }

}

void crearArchivoTemporal(char* nombreTabla, t_list* bloques) {

    int flag = 1;
    int dump = 1;

    while( flag == 1) {

        char* tmp = crear_path_tmp(nombreTabla, dump);

        if(ValidarArchivo(tmp) == false) {

            FILE * newFD;
            newFD = fopen(tmp, "w+");

            fwrite(TAMANIO_BLOQUE,1,strlen(TAMANIO_BLOQUE),newFD);

            char* bloquesDelArchivo = string_new();
            string_append(&bloquesDelArchivo, "BLOQUES=[");

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

            flag = 0;

        } else {

            dump++;
            free(tmp);
        }

    }
}

char* crear_path_tmp(char* tabla, int dump) {
    char* tmp = string_duplicate(montajeTablas);
    string_append(&tmp, "/");
    string_append(&tmp, tabla);
    string_append(&tmp, "/");
    char* str_dump = string_itoa(dump);
    string_append(&tmp, str_dump);
    string_append(&tmp, ".tmp");

    return tmp;
}

int getSizeRegistroTad(registro_tad* registro) {
    char* regKey = string_itoa(registro->key);
    char* regTime = string_itoa(registro->timestamp);
    char* regValue = registro->value;

    int totalSize = string_length(regKey)
            + string_length(regTime)
            + string_length(regValue)
            + (string_length(";") * 2)
            + 1; // \n

    return totalSize;
}

int getBloquesParaRegistro(registro_tad *registro) {
    int tamanioRegistro = getSizeRegistroTad(registro);
    return 1 + ((tamanioRegistro - 1) / TAMANIO_BLOQUES);
}

int getBloquesNecesarios(t_list* registros, t_list* bloquesAOcupar) {

    for(int j = 0; j < list_size(registros); j++) {

        int bloquesNecesarios = getBloquesParaRegistro((list_get(registros, j)));

        if(cantidad_bloquesLibres() >= bloquesNecesarios) {

            for(int i = 0; i < bloquesNecesarios; i++) {

                int bloqueLibre = bloque_libre();

                if(bloqueLibre != -1) {
                    bitarray_set_bit(bitarray, bloqueLibre);
                    list_add(bloquesAOcupar, (void*)bloqueLibre);
                } else {
                    //TODO: liberar los bloques asignados hasta el momento
                    return false;
                }

            }

        } else {
            return false;
        }
    }

    return true;
}



