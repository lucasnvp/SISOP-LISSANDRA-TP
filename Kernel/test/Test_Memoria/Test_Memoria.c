//
// Created by utnso on 20/05/19.
//

#include "Test_Memoria.h"

void agregar_tests_memoria() {
    CU_pSuite memoria = CU_add_suite("Memoria", inicializar_memoria, limpiar_memoria);
    CU_add_test(memoria, "test_memoria_existe_and_activa", test_memoria_existe_and_activa);
    CU_add_test(memoria, "test_memoria_existe_and_desactivada", test_memoria_existe_and_desactivada);
    CU_add_test(memoria, "test_memoria_no_existe", test_memoria_no_existe);
    CU_add_test(memoria, "test_search_memory", test_search_memory);
    CU_add_test(memoria, "test_disable_memory", test_disable_memory);
}

int inicializar_memoria() {
    LIST_MEMORIES = list_create();
    memory_tad* memory = memory_new(1, "127.0.0.1", 8080, 1);
    list_add(LIST_MEMORIES, memory);
    return 0;
}

int limpiar_memoria () {
    return 0;
}

void test_memoria_existe_and_activa () {
    inicializar_memoria();

    bool existe = exist_memory(1);
    CU_ASSERT_TRUE(existe);
}

void test_memoria_existe_and_desactivada () {
    inicializar_memoria();
    memory_tad* m1 = search_memory(1);
    disable_memory(m1);
    bool existe = exist_memory(1);
    CU_ASSERT_FALSE(existe);
}

void test_memoria_no_existe () {
    inicializar_memoria();

    bool existe = exist_memory(2);
    CU_ASSERT_FALSE(existe);
}

void test_search_memory () {
    inicializar_memoria();

    memory_tad* m1 = memory_new(1, "127.0.0.1", 8080, 1);
    memory_tad* m2 = search_memory(1);

    CU_ASSERT_EQUAL(m1->MEMORY_NUMBER, m2->MEMORY_NUMBER);
}

void test_disable_memory () {
    memory_tad* m1 = memory_new(1, "127.0.0.1", 8080, 1);

    disable_memory(m1);

    CU_ASSERT_FALSE(m1->ACTIVA);
}
