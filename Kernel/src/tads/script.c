//
// Created by utnso on 20/04/19.
//

#include "script.h"

script_tad* script_new(char* path){
    script_tad* auxScript = malloc(sizeof(script_tad));
    auxScript->lineas_ejecutadas = 0;
    auxScript->path = strdup(path);
    auxScript->fp = fopen(path, "r");
    return auxScript;
}