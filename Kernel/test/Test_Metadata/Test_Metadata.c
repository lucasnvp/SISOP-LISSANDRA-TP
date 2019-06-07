//
// Created by utnso on 07/06/19.
//

#include "Test_Metadata.h"

void agregar_tests_metadata() {
    CU_pSuite memoria = CU_add_suite("Metadata", inicializar_metadata, limpiar_metadata);
    CU_add_test(memoria, "test_load_metadata", test_load_metadata);
}

int inicializar_metadata() {
    init_METADATA();
    return 0;
}

int limpiar_metadata() {
    return 0;
}

void test_load_metadata () {
    inicializar_metadata();

    t_list* listDummy = list_create();
    describe_tad* d1 = new_describe_tad("tabla1", "SC", 1, 5);
    describe_tad* d2 = new_describe_tad("tabla2", "SHC", 2, 6);
    describe_tad* d3 = new_describe_tad("tabla3", "EC", 3, 7);
    list_add(listDummy, d1);
    list_add(listDummy, d2);
    list_add(listDummy, d3);

    load_METADATA(listDummy);
    print_metadata();
}