add_executable(KernelTests
        Run_Kernel_Tests.c
        Run_Kernel_Tests.h
        Test_Memoria/Test_Memoria.c
        Test_Memoria/Test_Memoria.h
        ../src/memoria/memoria.c
        ../src/memoria/memoria.h)

target_link_libraries (KernelTests PRIVATE cunit)
target_link_libraries (KernelTests PRIVATE Librerias)