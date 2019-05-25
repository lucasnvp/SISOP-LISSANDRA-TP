//
// Created by utnso on 20/04/19.
//

#ifndef TP_2019_1C_GANK_MID_SCRIPT_H
#define TP_2019_1C_GANK_MID_SCRIPT_H

#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct script_tad{
    uint32_t lineas_ejecutadas;
    char* path;
    FILE* fp;
}script_tad;

script_tad* script_new(char* path);

#endif //TP_2019_1C_GANK_MID_SCRIPT_H
