#include "tablesHandler.h"

void _dumpearTabla(char* nombreTabla, t_list* registros){

    pthread_mutex_lock(&SEM_MEMTABLE);
    lock_mx_drop(nombreTabla);
    lock_write_table(nombreTabla);

    uint64_t initTime = getCurrentTime();

    char* path = string_duplicate(montajeTablas);
    char* table = string_duplicate(nombreTabla);
    string_append(&path, table);

    if(ValidarArchivo(path)) {
        t_list* bloquesParaAsignar = list_create();

        char* registrosADumpear = transformAllRegistersToUniqueString(registros);

        int hayLugar = getBloquesNecesariosParaEscribirRegistros(registrosADumpear, bloquesParaAsignar);

        if(hayLugar == true) {
            crearArchivoTemporal(nombreTabla, bloquesParaAsignar, string_length(registrosADumpear));
            guardarEnBloques(registrosADumpear, bloquesParaAsignar);
            list_destroy(registros);
        } else {
            log_info(log_FileSystem, "FILE SYSTEM FULL ==> No hay bloques libres en este momento");
        }

        list_destroy(bloquesParaAsignar);
        free(registrosADumpear);
    }

    uint64_t finalTime = getCurrentTime();
    uint64_t diff = finalTime - initTime;

    log_info(log_FileSystem, "DUMP => Tabla <%s> tardo: <%lld>", nombreTabla, diff);

    free(path);
    free(table);

    unlock_rw_table(nombreTabla);
    unlock_mx_drop(nombreTabla);
    pthread_mutex_unlock(&SEM_MEMTABLE);
}

int getBloquesNecesariosParaEscribirRegistros(char *registros, t_list *bloquesAOcupar) {

    int bloquesNecesarios = cuantosBloquesOcupa(registros);

    return asignarBloquesParaRegistros(bloquesAOcupar, bloquesNecesarios);
}

char* transformAllRegistersToUniqueString(t_list *registros) {

    t_list* str_registros = list_map(registros, (void *) transformRegisterToString);

    char* value = (char*) list_fold(str_registros, string_new(), (void*) _concatenarRegistros);

    list_destroy(str_registros);

    return value;
}

char* transformRegisterToString(registro_tad *registro) {

    char* str_registro = string_new();
    char buf[256];
    snprintf(buf, sizeof buf, "%"PRIu64, registro->timestamp);
    string_append(&str_registro, buf);
    string_append(&str_registro, ";");
    string_append(&str_registro, string_itoa(registro->key));
    string_append(&str_registro, ";");
    string_append(&str_registro, registro->value);
    string_append(&str_registro, "\n");

    return str_registro;
}

char* _concatenarRegistros(char* seed, char* registro) {

    char* concat = string_new();
    concat = string_duplicate(seed);

    free(seed);

    string_append(&concat, registro);

    free(registro);

    return concat;
}

int cuantosBloquesOcupa(char* value) {

    int tamanio = string_length(value);

    return calcualarBloques(tamanio);
}

int calcualarBloques(int tamanio) {
    // Redondea hacia arriba
    return 1 + ((tamanio - 1) / TAMANIO_BLOQUES);
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

void crearArchivoTemporal(char* nombreTabla, t_list* bloques, int size) {

    int seguirHastaEncontrarElTmpCorrespondiente = true;
    int dump = 1;

    while( seguirHastaEncontrarElTmpCorrespondiente == true) {

        char* tmp = crear_path_tmp(nombreTabla, dump);

        if(ValidarArchivo(tmp) == false) {

            FILE * newFD;
            newFD = fopen(tmp, "w+");

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

            seguirHastaEncontrarElTmpCorrespondiente = false;

        } else {

            dump++;
            free(tmp);
        }

    }
}

void guardarEnBloques(char* value, t_list* bloques) {

    int limite = string_length(value);
    char* valorAGuardar = string_duplicate(value);

    for(int i = 0; i < list_size(bloques); i++) {

        char* pathBloque = crear_path_bloque((int) list_get(bloques, i));

        FILE * bloque = fopen(pathBloque, "w+");

        int limiteSuperior;

        if(limite >= TAMANIO_BLOQUES) {
            limiteSuperior = TAMANIO_BLOQUES;
        } else {
            limiteSuperior = limite;
        }

        char* take = string_substring(valorAGuardar, 0, limiteSuperior);

        fwrite(take,1,limite,bloque);

        limite -= string_length(take);

        if(limite > 0) {
            valorAGuardar = string_substring_from(valorAGuardar, limiteSuperior);
        }

        fclose(bloque);
        free(pathBloque);
        free(take);
    }

    free(valorAGuardar);
}

char* crear_path_bloque(int bloque) {
    char* nroBloque = string_duplicate(montajeBloques);
    char* str_nroBloque = string_itoa(bloque);
    string_append(&nroBloque, str_nroBloque);
    string_append(&nroBloque, ".bin");

    return nroBloque;
}
