//
// Created by utnso on 20/05/19.
//

#include "Run_Kernel_Tests.h"

int main () {
    CU_initialize_registry();

    agregar_tests_memoria();
    agregar_tests_criterio();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
