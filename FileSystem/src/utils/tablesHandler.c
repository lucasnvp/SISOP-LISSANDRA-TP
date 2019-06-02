
#include "tablesHandler.h"

void _dumpearTabla(char* nombreTabla, t_list* registros){

    t_list* bloquesParaAsignar = list_create();

    char* registroasADumpear = transformAllRegistersToUniqueString(registros);

    int hayLugar = getBloquesNecesariosParaDumpearTabla(registroasADumpear, bloquesParaAsignar);

    if(hayLugar == true) {
        crearArchivoTemporal(nombreTabla, bloquesParaAsignar);
        list_destroy(registros);
    } else {
        // TODO: que hacemos si no hay lugar?
    }

    list_destroy(bloquesParaAsignar);

}

int getBloquesNecesariosParaDumpearTabla(char* registros, t_list* bloquesAOcupar) {

    int bloquesNecesarios = cuantosBloquesOcupa(registros);

    return asignarBloquesParaRegistros(bloquesAOcupar, bloquesNecesarios);
}

char* transformAllRegistersToUniqueString(t_list *registros) {

    t_list* str_registros = list_map(registros, (void *) transformRegisterToString);

    return (char*) list_fold(str_registros, string_new(), (void*) _concatenarRegistros);
}

char* transformRegisterToString(registro_tad *registro) {

    char* str_registro = string_new();
    string_append(&str_registro, string_itoa(registro->timestamp));
    string_append(&str_registro, ";");
    string_append(&str_registro, string_itoa(registro->key));
    string_append(&str_registro, ";");
    string_append(&str_registro, registro->value);
    string_append(&str_registro, "\n");

    return str_registro;
}

char* _concatenarRegistros(char* seed, char* registro) {

    string_append(&seed, registro);

    return seed;
}

int cuantosBloquesOcupa(char* value) {

    int tamanio = string_length(value);

    return calcualarBloques(tamanio);
}

int calcualarBloques(int tamanio) {
    // Redondea hacia arriba
    return 1 + ((tamanio - 1) / TAMANIO_BLOQUES);
}

// Deprecated
int getBloquesParaRegistro(registro_tad *registro) {
    int tamanioRegistro = getSizeRegistroTad(registro);

    return calcualarBloques(tamanioRegistro);
}

// Deprecated
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

int asignarBloquesParaRegistros(t_list *bloquesNecesarios, int cantidad) {

    if(cantidad_bloquesLibres() >= cantidad) {

        for(int i = 0; i < cantidad; i++) {

            int bloqueLibre = bloque_libre();

            if(bloqueLibre != -1) {
                bitarray_set_bit(bitarray, bloqueLibre);
                list_add(bloquesNecesarios, (void*) bloqueLibre);
            } else {
                //TODO: liberar los bloques asignados hasta el momento
                return false;
            }

        }

    } else {
        return false;
    }

    return true;
}

void crearArchivoTemporal(char* nombreTabla, t_list* bloques) {

    int seguirHastaEncontrarElTmpCorrespondiente = true;
    int dump = 1;

    while( seguirHastaEncontrarElTmpCorrespondiente == true) {

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

            seguirHastaEncontrarElTmpCorrespondiente = false;

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



