add_executable(Kernel
        src/Kernel.c
        src/Kernel.h
        src/config/Config_kernel.c
        src/config/Config_kernel.h
        src/comandos/comandos.c
        src/comandos/comandos.h
        src/memoria/memoria.c
        src/memoria/memoria.h
        src/api/api.c
        src/api/api.h
        src/tads/script.c
        src/tads/script.h src/criterios/criterios.c src/criterios/criterios.h src/metadata/metadata.c src/metadata/metadata.h)

target_link_libraries (Kernel PRIVATE Librerias)
target_link_libraries (Kernel PRIVATE pthread)
target_link_libraries (Kernel PRIVATE readline)