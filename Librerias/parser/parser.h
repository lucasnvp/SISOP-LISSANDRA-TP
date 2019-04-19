//
// Created by utnso on 18/04/19.
//

#ifndef TP_2019_1C_GANK_MID_PARSER_H
#define TP_2019_1C_GANK_MID_PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include <commons/string.h>

typedef struct {
    bool valido;
    enum {
        SELECT,
        INSERT,
        CREATE,
        DESCRIBE,
        DROP
    } keyword;
    union {
        struct {
            char* tabla;
            uint16_t key;
        } SELECT;
        struct {
            char* tabla;
            uint16_t key;
            char* value;
        } INSERT;
        struct {
            char* tabla;
            char* consistencia;
            uint32_t particiones;
            uint32_t compactacion;
        } CREATE;
        struct {
            char* tabla;
        } DESCRIBE;
        struct {
            char* tabla;
        } DROP;
    } argumentos;
    char** _raw;
}t_lql_operacion;

t_lql_operacion parse(char* line);

void destruir_operacion(t_lql_operacion op);

#endif //TP_2019_1C_GANK_MID_PARSER_H
