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

insert_tad* new_insert_tad(char* nameTable, uint32_t key, char* value) {
    insert_tad* insert = malloc(sizeof(insert_tad));
    insert->nameTable = strdup(nameTable);
    insert->key = key;
    insert->value = strdup(value);
    return insert;
}

void free_insert_tad(insert_tad* insert){
    free(insert->nameTable);
    free(insert);
}