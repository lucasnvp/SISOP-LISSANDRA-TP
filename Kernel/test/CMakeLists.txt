add_executable(KernelTests
        Run_Kernel_Tests.c
        Run_Kernel_Tests.h
        Test_Memoria/Test_Memoria.c
        Test_Memoria/Test_Memoria.h
        ../src/memoria/memoria.c
        ../src/memoria/memoria.h
        Test_Criterio/Test_Criterio.c
        Test_Criterio/Test_Criterio.h
        ../src/criterios/criterios.c
        ../src/criterios/criterios.h
        Test_Metadata/Test_Metadata.c
        Test_Metadata/Test_Metadata.h
        ../src/metadata/metadata.c
        ../src/metadata/metadata.h)

target_link_libraries (KernelTests PRIVATE cunit)
target_link_libraries (KernelTests PRIVATE Librerias)