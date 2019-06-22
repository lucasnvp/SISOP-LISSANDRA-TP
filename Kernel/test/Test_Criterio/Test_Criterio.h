//
// Created by utnso on 23/05/19.
//

#ifndef TP_2019_1C_GANK_MID_TEST_CRITERIO_H
#define TP_2019_1C_GANK_MID_TEST_CRITERIO_H

#include <stdio.h>
#include <stdlib.h>

#include <commons/collections/list.h>

#include "../Run_Kernel_Tests.h"

void agregar_tests_criterio();
int inicializar_criterio();
int limpiar_criterio();

void test_criterio_sc();
void test_criterio_invalido();
void test_criterio_valido_and_memory_not_exists();
void test_criterio_filter();
void test_criterio_filter_random();

#endif //TP_2019_1C_GANK_MID_TEST_CRITERIO_H
