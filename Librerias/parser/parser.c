//
// Created by utnso on 18/04/19.
//

#include "parser.h"

#define RETURN_ERROR t_lql_operacion ERROR={ .valido = false }; return ERROR

#define SELECT_KEYWORD "SELECT"
#define INSERT_KEYWORD "INSERT"
#define CREATE_KEYWORD "CREATE"
#define DESCRIBE_KEYWORD "DESCRIBE"
#define DROP_KEYWORD "DROP"

#define CRITERIO_SC "SC"
#define CRITERIO_SHC "SHC"
#define CRITERIO_EC "EC"

t_lql_operacion parse(char* line){
    if(line == NULL || string_equals_ignore_case(line, "")){
//        fprintf(stderr, "No pude interpretar una linea vacia\n");
        RETURN_ERROR;
    }

    t_lql_operacion ret = {
            .valido = true
    };

    char* auxLine = string_duplicate(line);
    string_trim(&auxLine);
    char** split = string_n_split(auxLine, 5, " ");

    char* keyword = split[0];
    char* tabla = split[1];

    if(tabla == NULL){
//        fprintf(stderr, "No habia una tabla en la linea <%s>\n", (line)); \
		RETURN_ERROR;
    }

    ret._raw = split;

    if(string_equals_ignore_case(keyword, SELECT_KEYWORD)){
        ret.keyword = SELECT;
        ret.argumentos.SELECT.tabla = split[1];
        ret.argumentos.SELECT.key = atoi(split[2]);
    } else if(string_equals_ignore_case(keyword, INSERT_KEYWORD)){
        ret.keyword = INSERT;
        ret.argumentos.INSERT.tabla = split[1];
        ret.argumentos.INSERT.key= atoi(split[2]);
        ret.argumentos.INSERT.value= split[3];
    } else if(string_equals_ignore_case(keyword, CREATE_KEYWORD)){
        ret.keyword = CREATE;
        ret.argumentos.CREATE.tabla = split[1];
        if(string_equals_ignore_case(split[2], CRITERIO_SC)){
            ret.argumentos.CREATE.consistencia = CRITERIO_SC;
        } else if(string_equals_ignore_case(split[2], CRITERIO_SHC)){
            ret.argumentos.CREATE.consistencia = CRITERIO_SHC;
        } else if(string_equals_ignore_case(split[2], CRITERIO_EC)){
            ret.argumentos.CREATE.consistencia = CRITERIO_EC;
        } else {
            RETURN_ERROR;
        }
        ret.argumentos.CREATE.particiones = atoi(split[3]);
        ret.argumentos.CREATE.compactacion = atoi(split[4]);
    } else if(string_equals_ignore_case(keyword, DESCRIBE_KEYWORD)){
        ret.keyword = DESCRIBE;
        ret.argumentos.DESCRIBE.tabla = split[1];
    } else if(string_equals_ignore_case(keyword, DROP_KEYWORD)){
        ret.keyword = DROP;
        ret.argumentos.DROP.tabla = split[1];
    } else {
//        fprintf(stderr, "No se encontro el keyword <%s>\n", keyword);
        RETURN_ERROR;
    }

    free(auxLine);
    return ret;
}

void destruir_operacion(t_lql_operacion op){
    if(op._raw){
        string_iterate_lines(op._raw, (void*) free);
        free(op._raw);
    }
}