#include "estructuras.h"

registro_tad* new_registro_tad(uint32_t timestamp, uint32_t key, char* value){
    registro_tad* registro = malloc(sizeof(registro_tad));
    registro->timestamp = timestamp;
    registro->key = key;
    registro->value = strdup(value);
    return registro;
}

void free_registro_tad(registro_tad* registro){
    free(registro->value);
    free(registro);
}
