//
// Created by utnso on 20/04/19.
//

#include "script.h"

script_tad* script_new(uint32_t lineas_ejecutadas, char* path){
    script_tad* auxScript = malloc(sizeof(script_tad));
    auxScript->lineas_ejecutadas = lineas_ejecutadas;
    auxScript->path = strdup(path);
    return auxScript;
}