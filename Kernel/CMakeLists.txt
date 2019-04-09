add_executable(Kernel
        src/Kernel.c
        src/Kernel.h
        src/config/Config_kernel.c
        src/config/Config_kernel.h
        src/comandos/comandos.c
        src/comandos/comandos.h)

target_link_libraries (Kernel PRIVATE Librerias)
target_link_libraries (Kernel PRIVATE pthread)
target_link_libraries (Kernel PRIVATE readline)