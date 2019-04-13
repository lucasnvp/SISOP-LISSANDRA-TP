#include "estructuras.h"

registo_tad* new_registro_tad(uint32_t timestamp, uint32_t key, char* value){
    registo_tad* registro = malloc(sizeof(registo_tad));
    registro->timestamp = timestamp;
    registro->key = key;
    registro->value = strdup(value);
    return registro;
}

void free_registro_tad(registo_tad* registro){
    free(registro->value);
    free(registro);
}
