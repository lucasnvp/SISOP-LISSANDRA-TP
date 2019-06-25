//
// Created by utnso on 23/05/19.
//

#include "Test_Criterio.h"

void agregar_tests_criterio() {
    CU_pSuite criterio = CU_add_suite("Criterio", inicializar_criterio, limpiar_criterio);
    CU_add_test(criterio, "test_criterio_sc", test_criterio_sc);
    CU_add_test(criterio, "test_criterio_invalido", test_criterio_invalido);
    CU_add_test(criterio, "test_criterio_valido_and_memory_not_exists", test_criterio_valido_and_memory_not_exists);
    CU_add_test(criterio, "test_criterio_filter", test_criterio_filter);
    CU_add_test(criterio, "test_criterio_filter_random", test_criterio_filter_random);
}

int inicializar_criterio () {
    init_memories();
    add_memory(1,"127.0.0.1",8080,1);
    add_memory(2,"127.0.0.1",8080,2);
    add_memory(3,"127.0.0.1",8080,3);
    add_memory(4,"127.0.0.1",8080,4);
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

void test_criterio_valido_and_memory_not_exists () {
    inicializar_criterio();

    bool valido = criterio_add(10,"SC");
    CU_ASSERT_FALSE(valido);
}

void test_criterio_filter () {
    inicializar_criterio();
    bool valido;
    valido = criterio_add(1, "SC");
    valido = criterio_add(1, "SC");
    valido = criterio_add(1, "SC");
    valido = criterio_add(1, "SHC");

    t_list* filtered = criterio_search("SC");

    CU_ASSERT_EQUAL(3, list_size(filtered));
}

void test_criterio_filter_random () {
    inicializar_criterio();
    bool valido;
    valido = criterio_add(1, "SC");
    valido = criterio_add(2, "SC");
    valido = criterio_add(3, "SC");
    valido = criterio_add(4, "SHC");

    t_list* filtered = criterio_search("SC");
    CU_ASSERT_EQUAL(3, list_size(filtered));
    list_destroy(filtered);

    uint32_t index;
    uint32_t p1 = 0;
    uint32_t p2 = 0;
    uint32_t p3 = 0;
    printf("\n");
    for (int i = 0; i < 100; ++i) {
        index = criterio_ramdom_memory_by("SC");
        switch (index) {
            case 1:
                p1++;
                break;
            case 2:
                p2++;
                break;
            case 3:
                p3++;
                break;
        }
    }
    printf("P1: %d\t P2: %d\t P3: %d\t", p1, p2, p3);
    printf("\n");
}