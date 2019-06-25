project(Kernel_Test)

add_executable(KernelTests
        Run_Kernel_Tests.c
        Run_Kernel_Tests.h
        Test_Memoria/Test_Memoria.c
        Test_Memoria/Test_Memoria.h
        Test_Criterio/Test_Criterio.c
        Test_Criterio/Test_Criterio.h
        Test_Metadata/Test_Metadata.c
        Test_Metadata/Test_Metadata.h)

target_link_libraries (KernelTests PRIVATE cunit)
target_link_libraries (KernelTests PRIVATE Librerias)
target_link_libraries (KernelTests PRIVATE kernel_lib)