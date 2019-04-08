add_executable(Kernel
        src/Kernel.c
        src/Kernel.h
        src/config/Config_kernel.c
        src/config/Config_kernel.h
        )

target_link_libraries (Kernel PRIVATE Librerias)
target_link_libraries (Kernel PRIVATE pthread)