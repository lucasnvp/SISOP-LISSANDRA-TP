//
// Created by utnso on 07/06/19.
//

#ifndef TP_2019_1C_GANK_MID_TEST_METADATA_H
#define TP_2019_1C_GANK_MID_TEST_METADATA_H

#include <stdio.h>
#include <stdlib.h>

#include <commons/collections/list.h>
#include <serializador/estructuras.h>
#include <memoria/memoria.h>
#include <metadata/metadata.h>

#include "../Run_Kernel_Tests.h"

void agregar_tests_metadata();
int inicializar_metadata();
int limpiar_metadata();

void test_load_metadata();

#endif //TP_2019_1C_GANK_MID_TEST_METADATA_H
