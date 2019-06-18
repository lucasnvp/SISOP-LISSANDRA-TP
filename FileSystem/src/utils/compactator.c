#include "compactator.h"

void runCompactation(char* table) {

    t_list* registers = list_create();

    char* path = string_duplicate(montajeTablas);
    string_append(&path, table);

    if(ValidarArchivo(path) == true) {

        string_append(&path, "/");
        bool ok = tmpToTmpc(path);

        if(ok) {
            list_add_all(registers, getRegistersFromTemporals(table, ".tmpc"));
        }


    } else {

        log_info(log_FileSystem, "NO EXISTE LA TABLA %s", table);
        return;

    }

}

bool tmpToTmpc(char* path) {

    uint32_t mientrasExistanArchivos = true;
    uint32_t tmp = 1;
    bool ok = false;

    while(mientrasExistanArchivos == true) {

        char* pathTmp = string_duplicate(path);
        string_append(&pathTmp, string_itoa(tmp));
        string_append(&pathTmp, ".tmp");

        char* pathTmpC = string_duplicate(path);
        string_append(&pathTmpC, string_itoa(tmp));
        string_append(&pathTmpC, ".tmpc");

        if(ValidarArchivo(pathTmp) == true) {

            ok = rename(pathTmp, pathTmpC);

            tmp++;


        } else {

            mientrasExistanArchivos = false;

        }

        free(pathTmp);
        free(pathTmpC);
    }

    return ok;

}

