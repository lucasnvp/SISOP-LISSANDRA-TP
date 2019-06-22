project(Kernel_src)

add_library(kernel_lib STATIC
        config/Config_kernel.c
        config/Config_kernel.h
        comandos/comandos.c
        comandos/comandos.h
        memoria/memoria.c
        memoria/memoria.h
        api/api.c
        api/api.h
        tads/script.c
        tads/script.h
        criterios/criterios.c
        criterios/criterios.h
        metadata/metadata.c
        metadata/metadata.h
        metricas/metricas.c
        metricas/metricas.h)
target_include_directories(kernel_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (kernel_lib PRIVATE Librerias)

add_executable(Kernel
        Kernel.c
        Kernel.h
        )

target_link_libraries (Kernel PRIVATE Librerias)
target_link_libraries (Kernel PRIVATE pthread)
target_link_libraries (Kernel PRIVATE readline)
target_link_libraries (Kernel PRIVATE kernel_lib)