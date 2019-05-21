//
// Created by utnso on 20/05/19.
//

#ifndef TP_2019_1C_GANK_MID_TEST_MEMORIA_H
#define TP_2019_1C_GANK_MID_TEST_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>

#include "../Run_Kernel_Tests.h"
#include "../../src/memoria/memoria.h"

void agregar_tests_memoria();
int inicializar();
int limpiar();

void test_memoria_existe_and_activa();
void test_memoria_no_existe();

#endif //TP_2019_1C_GANK_MID_TEST_MEMORIA_H
