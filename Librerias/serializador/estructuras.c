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

select_tad* new_select_tad(char* nameTable, uint32_t key) {
    select_tad* select = malloc(sizeof(select_tad));
    select->nameTable = strdup(nameTable);
    select->key = key;
    return select;
}
void free_select_tad(select_tad* select) {
    free(select->nameTable);
    free(select);
}

create_tad* new_create_tad(char* nameTable, char* consistencia, u_int32_t particiones, u_int32_t compactacion) {
    create_tad* create = malloc(sizeof(create_tad));
    create->nameTable = strdup(nameTable);
    create->consistencia = strdup(consistencia);
    create->particiones = particiones;
    create->compactacion = compactacion;
    return create;
}

void free_create_tad(create_tad* create) {
    free(create->nameTable);
    free(create->consistencia);
    free(create);
}