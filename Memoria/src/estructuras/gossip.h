//
// Created by utnso on 12/06/19.
//

#ifndef TP_2019_1C_GANK_MID_GOSSIP_H
#define TP_2019_1C_GANK_MID_GOSSIP_H

#include <stdint-gcc.h>

typedef struct regGossip{
    uint32_t numeroMemoria;
    char* ip;
    uint32_t puerto;
}regGossip;

typedef struct tablaDeGossip{
    regGossip registro;
    struct tablaDeGossip* siguiente;
}tablaDeGossip;

struct tablaDeGossip* primerRegistroDeGossip;

void funcionGossip();

#endif //TP_2019_1C_GANK_MID_GOSSIP_H
