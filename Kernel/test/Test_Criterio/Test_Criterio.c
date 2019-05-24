//
// Created by utnso on 23/05/19.
//

#include "Test_Criterio.h"

void agregar_tests_criterio() {
    CU_pSuite criterio = CU_add_suite("Criterio", inicializar_criterio, limpiar_criterio);
    CU_add_test(criterio, "test_criterio_sc", test_criterio_sc);
    CU_add_test(criterio, "test_criterio_invalido", test_criterio_invalido);
}

int inicializar_criterio () {
    init_criterios();
    return 0;
}

int limpiar_criterio () {
    return 0;
}

void test_criterio_sc () {
    inicializar_criterio();

    bool valido = criterio_add(1,"SC");
    CU_ASSERT_TRUE(valido);
}

void test_criterio_invalido () {
    inicializar_criterio();

    bool valido = criterio_add(1,"ASD");
    CU_ASSERT_FALSE(valido);
}